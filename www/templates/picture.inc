<?
  $id = init_integer('id',0);
  $group = init_integer('group',0);
  $category = init_integer('category',0);
  
  $pid = init_integer('pid',0);

  $error = '';

  list($group_name) = mysql_fetch_row(mysql_query("select group_name from groups where group_id = '$group'"));
  list($category_name) = mysql_fetch_row(mysql_query("select category_name from categories where category_id = '$category'"));
  list($product_name) = mysql_fetch_row(mysql_query("select product_name from products where product_id = '$id'"));

  switch ( $action )
  {
    case 'delete': // Удаление Фотографии
      list($picture_name, $picture_name_big) = 
      	mysql_fetch_row(mysql_query("select picture_name, picture_name_big from product_pictures 
                                     where product_id = '$id' and picture_id = '$pid'"));
      if ( $picture_name && file_exists( $_SERVER['DOCUMENT_ROOT'].$product_additional_images.$picture_name ) )
        unlink($_SERVER['DOCUMENT_ROOT'].$product_additional_images.$picture_name);
      if ( $picture_name_big && file_exists( $_SERVER['DOCUMENT_ROOT'].$product_additional_big_images.$picture_name_big ) )
        unlink($_SERVER['DOCUMENT_ROOT'].$product_additional_big_images.$picture_name_big);

      mysql_query("delete from product_pictures 
                   where product_id = '$id' and picture_id = '$pid'");    	
      break;

    case 'addsave': // Сохранение добавленной Фотографии
    case 'editsave': // Сохранение сущуствующей Фотографии
      $new_title = init_string('new_title',''); $new_title = norm_string($new_title);
    	
      if ( !$new_title )
        $error = "Нельзя оставлять Фотографию без заголовка!";
      else
      {
          if ( $action == 'editsave' )	
          {
              mysql_query("update product_pictures set picture_title = '$new_title'
                           where product_id = '$id' and picture_id = '$pid'");
          }
          else
          {
              mysql_query("insert into product_pictures (product_id, picture_title, picture_name, picture_name_big)
                                                 values ('$id', '$new_title', '', '')");
              $pid = mysql_insert_id();
          }

          // Закачка файла с большой фотографией
          if ( $HTTP_POST_FILES['picture_big']['error'] != UPLOAD_ERR_NO_FILE )
          {
            $file = $HTTP_POST_FILES['picture_big']['tmp_name'];
            $user = $HTTP_POST_FILES['picture_big']['name'];
            $size = $HTTP_POST_FILES['picture_big']['size'];
            $type = $HTTP_POST_FILES['picture_big']['type'];
        
            $tfile = 'a'.$id.'-'.$pid.strrchr($user, '.');
			$tpath = $_SERVER['DOCUMENT_ROOT'].$product_additional_big_images;
            $tnpath = $_SERVER['DOCUMENT_ROOT'].$product_additional_images;
        
            if ( $type == 'image/jpeg' || $type == 'image/pjpeg' || $type == 'image/gif' || $type == 'image/x-png' || $type == 'image/png' )
            {
              if ( @move_uploaded_file($file,$tpath.$tfile) )
              {
				ResizeProductImage($tpath.$tfile, $tpath.$tfile, 1000, 1000);
                chmod($tpath.$tfile, 0644);
				ResizeProductImage($tpath.$tfile, $tnpath.$tfile, 200, 300);
				chmod($tnpath.$tfile, 0666);
				
                mysql_query("update product_pictures set picture_name_big = '$tfile', picture_name = '$tfile'
                             where product_id = '$id' and picture_id = '$pid'");
              }
              else $error = "Неизвестная ошибка при закачке файла $user!";
            }
            else $error = "Ошибка! Недопустимый тип файла $user!";
          }
      }
  }
      
  PrintHeader( "<p><a href=\"index.php?tab=group\">Группы товаров</a> - <a href=\"index.php?tab=category&group=$group\">$group_name</a> - <a href=\"index.php?tab=product&group=$group&category=$category\">$category_name</a> - <a href=\"index.php?tab=product&action=edit&group=$group&category=$category&id=$id\">$product_name</a> - Фотографии товара</p>" );

  if ( $error ) { ?>    	  
    <p class="baseerror"><?= $error ?></p>
<? } ?>
    <form id="addform" name="addform" method="post" enctype="multipart/form-data" onsubmit="return (addform.new_title.value.length>0 && addform.picture.value.length>0)">
      <input type="hidden" name="id" value="<?= $id ?>"/>
      <input type="hidden" name="action" value="addsave"/>
      <input type="hidden" name="tab" value="picture"/>
      <input type="hidden" name="group" value="<?= $group ?>"/>
      <input type="hidden" name="category" value="<?= $category ?>"/>
      <table width="100%" cellspacing="0" cellpadding="2" border="0" style="margin-top: 10px; margin-bottom: 10px">
        <tr>
          <td valign="top" style="width: 100px">
            <p>Заголовок</p>
          </td>
          <td valign="top" colspan="2">
            <input type="text" class="text" name="new_title" style="width: 500px" value=""/>
          </td>
          <td rowspan="2" align="center" valign="top">
            &nbsp;
          </td>
        </tr>
        <tr>
          <td valign="top">
            <p>Фотография</p>
          </td>
          <td valign="top" style="width: 300px">
        	<input class="text" type="file" name="picture_big" style="width: 300px; border: none;" size="44"/>
          </td>
          <td valign="top" style="width: 200px" align="right">
            <input type="submit" class="button" style="width: 120px" value="Добавить"/>
          </td>
        </tr>
      </table>
    </form>
    		  
    <div style="width: 100%; height: 1px; border-top: 1px solid #483D8B"/>
<?
    $pictures_sql = mysql_query("select * from product_pictures 
                                 where product_id = '$id'");
    
    for ( $i = 0; $i < mysql_num_rows($pictures_sql); $i++ )
    {
      $pictures = mysql_fetch_array($pictures_sql);

      $product_id = $pictures['product_id'];
      $picture_id = $pictures['picture_id'];
      $picture_name = $pictures['picture_name'];
      $picture_name_big = $pictures['picture_name_big'];
      $picture_title = $pictures['picture_title'];

      if ( !file_exists($_SERVER['DOCUMENT_ROOT'].$product_additional_images.$picture_name) ||
           !is_file($_SERVER['DOCUMENT_ROOT'].$product_additional_images.$picture_name) )
      $picture_name = $product_additional_picture_default;
      
		if ( !file_exists($_SERVER['DOCUMENT_ROOT'].$product_additional_big_images.$picture_name_big) ||
		 !is_file($_SERVER['DOCUMENT_ROOT'].$product_additional_big_images.$picture_name_big) )
		$picture_name_big = '';
?>
    <form id="form<?= $picture_id ?>" name="form<?= $picture_id ?>" method="post" enctype="multipart/form-data" onsubmit="return (form<?= $picture_id ?>.new_title.value.length>0)">
      <input type="hidden" name="id" value="<?= $product_id ?>"/>
      <input type="hidden" name="pid" value="<?= $picture_id ?>"/>
      <input type="hidden" name="action" value="editsave"/>
      <input type="hidden" name="tab" value="picture"/>
      <input type="hidden" name="group" value="<?= $group ?>"/>
      <input type="hidden" name="category" value="<?= $category ?>"/>
      <table width="100%" height="100" cellspacing="0" cellpadding="2" border="0" style="margin-top: 10px">
        <tr height="10">
          <td valign="top" style="width: 100px">
            <p>Заголовок</p>
          </td>
          <td valign="top" colspan="2">
            <input type="text" class="text" name="new_title" style="width: 500px" value="<?= $picture_title ?>"/>
          </td>
          <td rowspan="3" align="center" valign="top">
            <img src="<?= $product_additional_images.$picture_name ?>" alt="<?= $picture_title ?>"/>
<?		if ( $picture_name_big ) { ?>
			<br/><a href="<?= $product_additional_big_images.$picture_name_big ?>">Фотография</a>
<?		} ?>
          </td>
        </tr>
        <tr height="10">
          <td valign="top">
            <p>Фотография</p>
          </td>
          <td valign="top" style="width: 300px">
        	<input class="text" type="file" name="picture_big" style="width: 300px; border: none;" size="44"/>
          </td>
          <td valign="top" style="width: 200px" align="right">
            <input type="submit" class="button" style="width: 120px" value="Сохранить"/>
          </td>
        </tr>
      </table>
    </form>
<?      
    }
  
  function ResizeProductImage($original_picture, $preview_picture, $maxwidth = 120, $maxheight = 140)
  {
    list($width, $height, $type, $attr) = GetImageSize($original_picture);
    
    if ( $height > $width ) {
      $nheight = $maxheight; $nwidth = round( $width * $maxheight / $height ); }
    else {
      $nwidth = $maxwidth; $nheight = round( $height * $maxwidth / $width ); }
    
    switch ( $type )
    {
      case 1: $si = ImageCreateFromGif($original_picture); break;
      case 2: $si = ImageCreateFromJpeg($original_picture); break;
      case 3: $si = ImageCreateFromPng($original_picture); break;
    }
    
    $di = ImageCreateTrueColor($nwidth, $nheight);  
	
	if ( $type == 1 ) {
		imagecolortransparent($di, imagecolorat($di, 0, 0));
	}
	if ( $type == 3 ) {
		imagesavealpha($di, true); imagealphablending($di, false);
	}
	
    ImageCopyResampled($di,$si,0,0,0,0,$nwidth,$nheight,$width,$height);
    
    switch ( $type )
    {
      case 1: ImageGif($di,$preview_picture); break;
      case 2: ImageJpeg($di,$preview_picture); break;
      case 3: ImagePng($di,$preview_picture); break;
    }
    
    ImageDestroy($si); ImageDestroy($di);
  }
?>