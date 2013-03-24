<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml">
	<head>
		<title><?= $title ?></title>
		<meta http-equiv="Content-Type" content="text/html; charset=windows-1251"/>
		<meta name="title" content="<?= $title ?>">
		<meta name="keywords" content="<?= $keywords ?>">
		<meta name="description" content="<?= $description ?>">
		<link rel="icon" href="/favicon.ico" />
		<link rel="stylesheet" href="/styles/index.css" type="text/css"/>
		<script type="text/javascript" src="/scripts/jquery-1.9.1.min.js"></script>
	</head>
	<body>
		<div class="<?= $_SERVER['SCRIPT_NAME'] == '/index.php' ? 'index' : 'default' ?>">
			<div class="top">
				<a href="/" class="index"></a>
				<a href="/articles.php" class="articles"></a>
				<a href="/delivery.php" class="delivery"></a>
				<a href="/about.php" class="about"></a>
				<a href="mailto:<?= get_preference('manager_email', 'andreisport@bodysolid.ru') ?>" class="email"></a>
				<div class="card">
					<a href="/basket.php">корзина</a> (<?= @array_sum($_SESSION['_products']) ?> шт.)
				</div>
				<div class="search">
					<form action="/search.php" method="post">
						<input type="text" class="text" name="search_text" value=""/>
						<input type="submit" class="button" value=""/>
					</form>
				</div>
			</div>
			<div class="inner">
				<div class="catalogue">
					<div class="catalogue_top"></div>
					<div class="catalogue_center">
						<script type="text/javascript">
							function toggle_menu(item) {
								if ($(item).hasClass('selected')) {
									$(item).removeClass('selected');
									$(item).next().hide(400);
								} else {
									$(item).siblings().removeClass('selected');
									$(item).parent().find('div').hide(400);
									$(item).addClass('selected');
									$(item).next().show(400);
								}
							}
						</script>
<?
	$product_id = $category_id = $group_id = 0;
	if ($_SERVER['SCRIPT_NAME'] == '/products.php') {
		$tab = init_string('tab', 'group');
		switch ( $tab ) {
			case 'product':
				$product_id = init_integer('product_id', 0);
				list($category_id) = mysql_fetch_row(mysql_query("select category_id from products where product_id = '$product_id'"));
				list($group_id) = mysql_fetch_row(mysql_query("select group_id from categories where category_id = '$category_id'"));
				break;
			case 'category':
				$category_id = init_integer('category_id', 0);
				list($group_id) = mysql_fetch_row(mysql_query("select group_id from categories where category_id = '$category_id'"));
				break;
			case 'group':
				$group_id = init_integer('group_id', 0);
		}
	}
	
	$menu_rows = array();
	
	$groups_sql = mysql_query("select * from groups where group_active = '1' order by group_order");
	while ( $groups_row = mysql_fetch_array($groups_sql) ) {
		$menu_rows[ $groups_row['group_id'] ] = array( 'group_name' => $groups_row['group_name'], 'group_categories' => array() );
		$categories_sql = mysql_query("select * from categories where group_id = '".$groups_row['group_id']."' and category_active = '1' order by category_order");
		while ( $categories_row = mysql_fetch_array($categories_sql) ) {
			$menu_rows[$groups_row['group_id']]['group_categories'][$categories_row['category_id']] = array( 'category_name' => $categories_row['category_short_name'] );
		}
	}
	foreach ( $menu_rows as $group_index => $group_value ) {
?>
						<a href="" onclick="toggle_menu(this); return false;"<? if ($group_index == $group_id) { ?> class="selected"<? } ?>><?= $menu_rows[$group_index]['group_name'] ?></a>
						<div<? if ($group_index != $group_id) { ?> style="display: none"<? } ?>>
<?
		foreach ( $menu_rows[$group_index]['group_categories'] as $category_index => $category_value ) {
?>
							<a href="products.php?tab=category&amp;category_id=<?= $category_index ?>" class="submenu<? if ($category_index == $category_id) { ?> selected<? } ?>"><?= $menu_rows[$group_index]['group_categories'][$category_index]['category_name'] ?></a>
<?
		}
?>
						</div>
<?
	}
?>
					</div>
					<div class="catalogue_bottom"></div>
				</div>
				<div class="content">
					<div class="content_top"></div>
					<div class="content_center">
