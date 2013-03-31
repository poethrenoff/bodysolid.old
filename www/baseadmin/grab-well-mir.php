<?
	require "../templates/common.inc";
	$document_root = dirname(__FILE__) . '/../';
	
	$main_url = 'http://www.well-mir.ru';
	
	/* OCTANE */
	$category_id = 108;
	$category_url = $main_url . '/makers110/&p=all';
	$brand_id = 32;
	
	/* CYBEX */
	// $category_id = 107;
	// $category_url = $main_url . '/makers38/&p=all';
	// $brand_id = 17;
	
	$category_page = file_get_contents($category_url);
	
	preg_match_all("/\<a href=\"(?<product_url>\/goods\/(?<product_id>\d+)\.htm)\"\>\<img src=\".+\"\>\<\/a\>/isU", $category_page, $product_matches, PREG_SET_ORDER);
	foreach ($product_matches as $product_match) {
		$product_id = $product_match['product_id'];
		$product_url = $main_url . $product_match['product_url'];
		$product_image = $main_url . '/uploads/goods/570x465/' . $product_id . '.jpg';
		$product_preview = $main_url . '/uploads/goods/160x135/' . $product_id . '.jpg';
		
		$product_page = file_get_contents($product_url);
		
		preg_match("/\<h1.+\>(.+)\<\/h1\>/isU", $product_page, $product_name_match);
		$product_name = norm_html($product_name_match[1]);
		
		preg_match_all("/\<a href=\"(?<photo_image_url>\/uploads\/goods_youtube\/.+)\"  id = \"thumb\d+\"\s+onclick=\"return false;\"\>\s+\<img src=\"(?<photo_preview_url>\/uploads\/goods_youtube\/.+)\"/isU", $product_page, $photo_matches, PREG_SET_ORDER);
		
		preg_match("/\<div id=\"divtext1\".+>(?<product_description>.+)\<div id=\"divtext2\".+\>/isU", $product_page, $product_description_match);
		$product_description = $product_description_match['product_description'];
		$product_description = preg_replace('/\<a href=\"\/.+\"\>(.+)\<\/a\>/isU', '\1', $product_description);
		$product_description = preg_replace('/\<a href=\"' . preg_quote($main_url, '/') . '.+\"\>(.+)\<\/a\>/isU', '\1', $product_description);
		
		preg_match_all("/\<iframe.+\>\<\/iframe>/isU", $product_description, $video_matches, PREG_SET_ORDER);
		$product_video = array();
		foreach ($video_matches as $video_match) {
			$product_video[] = $video_match[0];
		}
		$product_video = norm_html(join('', $product_video));
		
		$product_description = str_replace(' class="MsoNormal"', '', $product_description);
		$product_description = str_replace('<p>&nbsp;</p>', '', $product_description);
		$product_description = str_replace('<div>&nbsp;</div>', '', $product_description);
		$product_description = strip_tags($product_description, '<p><strong><br><a><ul><ol><li><b><i>');
		$product_description = trim(norm_html($product_description));
		
		preg_match("/\<div id=\"divtext2\".+>(?<product_features>.+)\<div id=\"divtext3\".+\>/isU", $product_page, $product_features_match);
		$product_features = $product_features_match['product_features'];
		$product_features = preg_replace('/\<iframe.+\<\/div\>/isU', '', $product_features);
		$product_features = preg_replace("/\<p.*\>\<strong.*\>Цвета, доступные для тренажеров этой серии, на примере данной модели:\<\/strong\>\<\/p\>/isU", '', $product_features);
		$product_features = str_replace('<p>&nbsp;</p>', '', $product_features);
		$product_features = str_replace('<div>&nbsp;</div>', '', $product_features);
		$product_features = strip_tags($product_features, '<table><tbody><tr><td><p><strong><a><b><i>');
		$product_features = trim(norm_html($product_features));
		
		$product_search = norm_search($product_name).' '.norm_search($product_features).' '.norm_search($product_description);
		
		mysql_query("insert into products (product_name, category_id, brand_id, product_price, product_picture, product_picture_big,
					 product_short_desc, product_full_desc, product_video, product_search, product_addon)
					 values ('{$product_name}', '{$category_id}', '{$brand_id}', '0', '', '',
					 '', '" . $product_features . $product_description . "', '{$product_video}', '{$product_search}', '')");
		
		$id = mysql_insert_id();
		$tfile = 'p' . $id . '.jpg'; 
		$tnpath = $document_root . $product_images;
		$tprpath = $document_root . $product_preview_images;
		
		file_put_contents($tnpath . $tfile, file_get_contents($product_image));
		file_put_contents($tprpath . $tfile, file_get_contents($product_preview));
		
		mysql_query("update products set product_picture = '$tfile' where product_id = '$id'");
		
		foreach ($photo_matches as $photo_match) {
			$photo_image_url = $main_url . $photo_match['photo_image_url'];
			$photo_preview_url = $main_url . $photo_match['photo_preview_url'];
			
			mysql_query("insert into product_pictures (product_id, picture_title, picture_name, picture_name_big)
						 values ('{$id}', '', '', '')");
			$pid = mysql_insert_id();
			
			$tfile = 'a' . $id . '-' . $pid . '.jpg';
			$tpath = $document_root . $product_additional_big_images;
			$tnpath = $document_root . $product_additional_images;
			
			file_put_contents($tpath . $tfile, file_get_contents($photo_image_url));
			file_put_contents($tnpath . $tfile, file_get_contents($photo_preview_url));
			
			mysql_query("update product_pictures set picture_name_big = '{$tfile}', picture_name = '{$tfile}'
						 where product_id = '{$id}' and picture_id = '{$pid}'");
		}
	}
