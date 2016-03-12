<?	
	require 'function.inc';

	$category_id = init_integer('category_id', 0);
	$product_order = init_string('product_order', 'date');
	
	$categories_sql = mysql_query("select * from categories where category_id = '$category_id'");

	if ( $categories_row = mysql_fetch_array($categories_sql) )
	{
		$category_name = $categories_row['category_name'];
		$category_comment = $categories_row['category_comment'];
		$category_comment = $categories_row['category_comment'];

		$category_comment = '<p class="p">'.join('</p><p class="p">', explode( "\r\n\r\n", trim($category_comment) ) ).'</p>';

		list($group_id, $group_name) = mysql_fetch_row( mysql_query(
			"select groups.group_id, groups.group_name
			 from groups inner join categories on 
			 categories.group_id = groups.group_id where categories.category_id = '$category_id'") );
?>
<p class="path"><a class="path" href="?tab=group&amp;group_id=<?= $group_id ?>"><?= $group_name ?></a> - <?= $category_name ?></p>

<h1><?= $category_name ?></h1>

<?
		switch ( $product_order )
		{
			case 'name': $product_order_select = 'product_name'; break;
			case 'price': $product_order_select = 'product_price'; break;
			default: $product_order_select = 'product_id desc';
		}
?>

<form method="get" id="sort" action="<?= $_SERVER['PHP_SELF'] ?>" style="margin-top: -15px">
	<table cellpadding="2" cellspacing="0" style="width: 100%">
		<tr>
			<td valign="top" style="padding-top: 6px; text-align: right">
				Сортировать по
			</td>
			<td style="width: 100px">
				<input type="hidden" name="tab" value="category"/>
				<input type="hidden" name="category_id" value="<?= $category_id ?>"/>
				<select name="product_order" onchange="document.forms['sort'].submit()" style="width: 100%">
					<option value="date">дате</option>
					<option value="name"<?= ( $product_order == 'name' )?' selected="selected"':'' ?>>имени</option>
					<option value="price"<?= ( $product_order == 'price' )?' selected="selected"':'' ?>>цене</option>
				</select>
			</td>
		</tr>
	</table>
</form>
<?
		$products_sql = mysql_query("select * from products 
									 where category_id = '$category_id' and product_active = '1'
									 order by ".$product_order_select);

		if ( mysql_num_rows($products_sql) )
		{
			$products_rows = array();
			while ( $row = mysql_fetch_array($products_sql) )
				$products_rows[] = $row;
			
			PrintProducts($products_rows, $categories_row, true, $categories_row['group_id'] == 5);
		}
	}
?>