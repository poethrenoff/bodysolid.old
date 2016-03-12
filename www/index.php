<?
	require 'templates/common.inc';
	require 'templates/function.inc';

	list($title, $keywords, $description) = get_head('index', 0);

	require 'templates/header.inc';

	$categories_sql = mysql_query("select * from categories 
								 where group_id = 5 and category_active = '1'
								 order by rand() limit 1");
	$categories_row =  mysql_fetch_array($categories_sql);
	
	$products_sql = mysql_query("select * from products 
								 where category_id = '" . $categories_row['category_id'] . "' and product_active = '1'
								 order by product_id desc");
?>
<h1><?= $categories_row['category_name'] ?></h1>
<?
	if ( mysql_num_rows($products_sql) )
	{
		$products_rows = array();
		while ( $row = mysql_fetch_array($products_sql) ) {
			$products_rows[] = $row;
		}
		
		PrintProducts($products_rows, $categories_row, false, true);
	}

	//require 'templates/index.inc';

	require 'templates/footer.inc';
?>