<?
	Header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");
	Header("Cache-Control: no-cache, must-revalidate");
	Header("Pragma: no-cache");
	Header("Last-Modified: ".gmdate("D, d M Y H:i:s")." GMT");
	
	print '<?xml version="1.0" encoding="windows-1251"?>'."\n";
	print '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">'."\n";
?>
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="ru" lang="ru">
	<head>
		<title><?= $title ?></title>
		<meta name="keywords" content="<?= $keywords ?>"/>
	   	<meta name="description" content="<?= $description ?>"/>
		<meta http-equiv="Content-Type" content="text/html; charset=windows-1251"/>
		<link rel="stylesheet" href="styles/index.css" type="text/css"/>
		<link rel="shortcut icon" href="favicon.ico">
	</head>
	<body>
		<table cellpadding="0" cellspacing="0" style="width: 100%">
			<tr align="center">
				<td>
					<table cellpadding="0" cellspacing="0" style="width: 980px">
						<tr>
							<td colspan="2" style="width: 980px; height: 415px; background-image: url(/images/design/design_r1_c1.jpg)" align="left" valign="top">
								<div style="position: relative; top: 0px; left: 450px; width: 530px">
									<table cellpadding="0" cellspacing="0" style="width: 100%">
										<tr>
											<td style="width: 260px" align="left" valign="top">
												<p class="main_menu" style="margin-top: 100px"><a href="index.php">Силовые тренажеры<br/>BodySolid</a></p>
												<p class="main_menu" style="margin-top: 15px"><a href="articles.php">Особенности</a></p>
												<p class="main_menu" style="margin-top: 15px"><a href="about.php">Где купить</a></p>
												<table cellpadding="0" cellspacing="0" style="width: 100%; height: 25px; margin-top: 155px">
													<tr>
														<td style="width: 20px" align="center" valign="middle">
															<img src="/images/design/search.gif"/>
														</td>
														<td align="center" valign="top">
															<form method="post" id="search" action="search.php" onsubmit="return forms['search'].search_text.value.length > 0">
																<input type="text" class="text" name="search_text" value="Введите слова для поиска" onclick="this.value = ''" style="width: 95%"/>
															</form>
														</td>
													</tr>
												</table>
											</td>
											<td align="left" valign="top">
												<div style="position: relative; top: 25px; left: 50px; width: 190px">
													<p class="contacts">Наши телефоны:</p>
													<p class="contacts_phone" style="margin-top: -5px">(495) 518-52-97<br/>(495) 518-14-19</p>
													<p class="contacts" style="margin-left: 80px;"><br/><a href="basket.php">Ваша корзина</a></p>
												</div>
											</td>
										</tr>
									</table>
								</div>
							</td>
						</tr>
						<tr>
							<td style="width: 280px" align="left" valign="top">
<?
	$menu_rows = array();

	$groups_sql = mysql_query("select * from groups where group_active = '1' order by group_order");
	while ( $groups_row = mysql_fetch_array($groups_sql) )
	{
		$menu_rows[ $groups_row['group_id'] ] = array( 'group_name' => $groups_row['group_name'], 'group_categories' => array() );
		
		$categories_sql = mysql_query("select * from categories where group_id = '".$groups_row['group_id']."' and category_active = '1' order by category_order");
		while ( $categories_row = mysql_fetch_array($categories_sql) )
		{
			$menu_rows[$groups_row['group_id']]['group_categories'][$categories_row['category_id']] = array( 'category_name' => $categories_row['category_short_name'] );
		}
	}
?>
											<table cellpadding="1" cellspacing="0" style="width: 190px; margin: 0px 20px 0px 12px">
<?
	foreach ( $menu_rows as $group_index => $group_value )
	{
		$group_name = $menu_rows[$group_index]['group_name'];
?>
												<tr>
													<td>
														<p class="menu_group"><b><a href="products.php?tab=group&amp;group_id=<?= $group_index ?>"><?= $group_name ?></a></b></p>
													</td>
												</tr>
<?
		foreach ( $menu_rows[$group_index]['group_categories'] as $category_index => $category_value )
		{
			$category_name = $menu_rows[$group_index]['group_categories'][$category_index]['category_name'];
?>
												<tr>
													<td>
														<p class="menu_category"> - <a href="products.php?tab=category&amp;category_id=<?= $category_index ?>"><?= $category_name ?></a></p>
													</td>
												</tr>
<?
		}
	}
?>
											</table>
							</td>
							<td style="width: 700px; padding-top: 30px;  padding-bottom: 10px; background-image: url(/images/design/design_r2_c2.jpg); background-position: right top; background-repeat: no-repeat" valign="top">
