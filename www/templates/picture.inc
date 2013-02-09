<?
	$product_id = init_integer('product_id', 0);
	$picture_id = init_integer('picture_id', 0);
	
	$products_sql = mysql_query("select * from products where product_id = '$product_id'");
	
	if ( $products_row = mysql_fetch_array($products_sql) )
	{
		$product_name = $products_row['product_name'];
		$product_picture_big = $products_row['product_picture_big'];
		
		$title = $keywords = $description = $product_name;
		
		if ( !file_exists($_SERVER['DOCUMENT_ROOT'].$product_big_images.$product_picture_big) ||
			 !is_file($_SERVER['DOCUMENT_ROOT'].$product_big_images.$product_picture_big) )
			$product_picture_big = '';
		
		$picture_name_big = '';
		
		if ( $picture_id )
		{
			$pictures_sql = mysql_query("select * from product_pictures where picture_id = '$picture_id'");
			
			if ( $pictures_row = mysql_fetch_array($pictures_sql) )
			{
				$picture_title = $pictures_row['picture_title'];
				$picture_name_big = $pictures_row['picture_name_big'];
				
				$title = $keywords = $description = $product_name.' :: '.$picture_title;
				
				if ( !file_exists($_SERVER['DOCUMENT_ROOT'].$product_additional_big_images.$picture_name_big) &&
					 !is_file($_SERVER['DOCUMENT_ROOT'].$product_additional_big_images.$picture_name_big) )
					$picture_name_big = '';
			}
		}
	
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
	</head>
	<body>
		<table cellpadding="0" cellspacing="0" style="width: 100%">
			<tr align="center">
				<td>
<?		if ( $picture_name_big ) { ?>
					<img src="<?= $product_additional_big_images.$picture_name_big ?>" class="product" alt="<?= $picture_title ?>" title="<?= $picture_title ?>" style="margin-top: 50px"/>
<?		} else if ( $product_picture_big ) { ?>
					<img src="<?= $product_big_images.$product_picture_big ?>" class="product" alt="<?= $product_name ?>" title="<?= $product_name ?>" style="margin-top: 50px"/>
<?		} ?>
				</td>
			</tr>
		</table>
	</body>
</html>
<?
	}
?>
