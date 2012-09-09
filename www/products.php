<?
	require 'templates/common.inc';

	$tab = init_string('tab', 'group');
	
	$course = get_preference('course', 28.00);
	$discount_limit = get_preference('discount_limit', 1000.00);
	$discount_value = get_preference('discount_value', 0.9);

	switch ( $tab )
	{
		case 'product':
			$product_id = init_integer('product_id', 0);
			list($product_name) = mysql_fetch_row(mysql_query("select product_name from products where product_id = '$product_id'"));
			$title = $keywords = $description = $product_name;
			break;

		case 'category':
			$category_id = init_integer('category_id', 0);
			list($title, $keywords, $description) = get_head('category', $category_id);
			break;

		default:
			$group_id = init_integer('group_id', 0);
			list($title, $keywords, $description) = get_head('group', $group_id);
	}

	require 'templates/header.inc';
	
	switch ( $tab )
	{
		case 'product':
			include('templates/product.inc');
			break;

		case 'category':
			include('templates/category.inc');
			break;

		default:
			include('templates/group.inc');
	}

	require 'templates/footer.inc';
?>