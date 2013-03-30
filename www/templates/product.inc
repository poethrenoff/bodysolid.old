<?	
	$product_id = init_integer('product_id', 0);
	
	$products_sql = mysql_query("select * from products where product_id = '$product_id' and product_active = 1");
	
	if ( $products_row = mysql_fetch_array($products_sql) )
	{
		$product_name = $products_row['product_name'];
		$product_price = $products_row['product_price'];
		$product_short_desc = $products_row['product_short_desc'];
		$product_full_desc = $products_row['product_full_desc'];
		$product_video = $products_row['product_video'];
		$product_picture = $products_row['product_picture'];
		$product_instruct = $products_row['product_instruct'];
		$product_picture_big = $products_row['product_picture_big'];
		$product_no_discount = $products_row['product_no_discount'];
		$product_addon = $products_row['product_addon'];
		
		if ( !file_exists($_SERVER['DOCUMENT_ROOT'].$product_images.$product_picture) ||
			 !is_file($_SERVER['DOCUMENT_ROOT'].$product_images.$product_picture) )
			$product_picture = $product_picture_default;
		
		list($category_id, $category_name, $category_by_course) = mysql_fetch_row( mysql_query(
			"select categories.category_id, categories.category_name, category_by_course
			 from categories inner join products on 
			 products.category_id = categories.category_id where products.product_id = '$product_id'") );

		list($group_id, $group_name) = mysql_fetch_row( mysql_query(
			"select groups.group_id, groups.group_name
			 from groups inner join categories on 
			 categories.group_id = groups.group_id where categories.category_id = '$category_id'") );
		
		$product_price = round($category_by_course ? $product_price * $course : $product_price);
		if ( !$product_no_discount && $product_price > $discount_limit )
			$product_price *= $discount_value;
?>
<p class="path"><a class="path" href="?tab=group&amp;group_id=<?= $group_id ?>"><?= $group_name ?></a> - <a class="path" href="?tab=category&amp;category_id=<?= $category_id ?>"><?= $category_name ?></a> - <?= $product_name ?></p>

<h1><?= $product_name ?></h1>

<script type="text/javascript">
	function toggle_desc(id) {
		if ($('#' + id).css('display') != 'block') {
			$('.desc').hide(400);
			$('#' + id).show(400);
		}
	}
</script>
<table cellpadding="2" cellspacing="0" style="width: 100%">
	<tr valign="top">
		<td style="width: 160px; text-align: center">
			<img src="<?= $product_preview_images.$product_picture ?>" alt="<?= $product_name ?>" title="<?= $product_name ?>"/>
			<table cellpadding="0" cellspacing="0" style="width: 100%; margin-top: 10px">
				<tr valign="middle">
					<td style="width: 80%">
						<p>Цена: <b><?= $product_price ?> р.</b></p>
					</td>
					<td style="width: 20%" align="right">
						<a href="basket.php?action=add&amp;product_id=<?= $product_id ?>" style="margin-right: 10px"><img src="images/basket.gif" alt="Купить" title="Купить"/></a>
					</td>
				</tr>
			</table>
			<br/><br/>
			<a href="" onclick="toggle_desc('desc'); return false;">ОПИСАНИЕ</a>
			<br/><br/>
			<a href="" onclick="toggle_desc('image'); return false;">ИЗОБРАЖЕНИЯ</a>
<?
		if ( $product_video ) 
		{
?>
			<br/><br/>
			<a href="" onclick="toggle_desc('video'); return false;">ВИДЕО</a>
<?
		}
?>
		</td>
		<td>
			<div id="desc" class="desc" style="display: block">
				<p class="p"><?= $product_short_desc ?></p><?= $product_full_desc ?>
<?
		if ( $product_instruct && file_exists($_SERVER['DOCUMENT_ROOT'].$product_instructions.$product_instruct) ) 
		{
?>
			<p class="p"><b><a href="<?= $product_instructions.$product_instruct ?>">Скачать инструкцию</a></b></p>
<?
		}
		
		if ( $product_addon ) 
		{
?>
			<p class="p" style="color: #c70711"><b>Опции к тренажерам</b></p>
<?
			$addon_sql = mysql_query("select product_id, product_name from products where product_id in ( $product_addon )");
			
			while ( $addon_row = mysql_fetch_array($addon_sql) )
			{
				$product_id = $addon_row['product_id'];
				$product_name = $addon_row['product_name'];
?>
			<p class="p"><a href="?tab=product&amp;product_id=<?= $product_id ?>"><?= $product_name ?></a></p>
<?
			}
		}
?>
			</div>
			<div id="image" class="desc" style="display: none; text-align: center">
<?		if ( $product_picture_big ) { ?>
				<a href="picture.php?product_id=<?= $product_id ?>"><img src="<?= $product_images.$product_picture ?>" class="product" alt="<?= $product_name ?>" title="<?= $product_name ?>"/></a>
<?		} else { ?>
				<img src="<?= $product_images.$product_picture ?>" class="product" alt="<?= $product_name ?>" title="<?= $product_name ?>"/>
<?		} ?>
				<br/><br/>
<?
		$pictures_sql = mysql_query("select * from product_pictures where product_id = '$product_id'");
		
		$pictures_cnt = mysql_num_rows($pictures_sql);
		
		if ( $pictures_cnt )
		{
			while ( $pictures_row = mysql_fetch_array($pictures_sql) )
			{
				$picture_id = $pictures_row['picture_id'];
				$picture_name = $pictures_row['picture_name'];
				$picture_name_big = $pictures_row['picture_name_big'];
				$picture_title = $pictures_row['picture_title'];

				if ( !file_exists($_SERVER['DOCUMENT_ROOT'].$product_additional_images.$picture_name) ||
					 !is_file($_SERVER['DOCUMENT_ROOT'].$product_additional_images.$picture_name) )
				$picture_name = $product_additional_picture_default;
?>
		<div class="picture">
<?		if ( $picture_name_big ) { ?>
			<a href="picture.php?product_id=<?= $product_id ?>&amp;picture_id=<?= $picture_id ?>"><img src="<?= $product_additional_images.$picture_name ?>" alt="<?= $picture_title ?>"/></a>
<?		} else { ?>
            <img src="<?= $product_additional_images.$picture_name ?>" alt="<?= $picture_title ?>"/>
<?		} ?>
    		<p class="c"><?= $picture_title ?></p>
		</div>
<?
			}
		}
?>
				<div class="clear"></div>
			</div>
<?
		if ( $product_video ) 
		{
?>
			<div id="video" class="desc" style="display: none; text-align: center">
				<?= $product_video ?>
			</div>
<?
		}
?>
		</td>
	</tr>
</table>
<?
	}
?>
