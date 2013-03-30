<?
        $id = init_integer('id',0);
        $page = init_integer('page',0);
        $group = init_integer('group',0);
        $category = init_integer('category',0);
        
        $course = get_preference('course', 28.00);
        
        $active_only = init_string('active_only','false');
        if ( $active_only != 'true' && $active_only != 'false' ) $active_only = 'false';

        $sort = init_string('sort','id');
        if ( $sort != 'id' && $sort != 'name' && $sort != 'price' ) $sort = 'id';

        $order = init_string('order','desc');
        if ( $order != 'desc' && $order != 'asc' ) $order = 'desc';
        
        $next_order = ( $order == 'desc' )?'asc':'desc';

        $error = '';

        switch ( $action )
        {
          case 'saveprice': // Сохранение цены товара
              $new_price = init_float('new_price', -1);
              if ( $new_price >= 0 )
              {
                mysql_query("update products set product_price = '$new_price'
                             where product_id = '$id'");
              }
              else $error = "Неверное значение цены товара!";
    	    break;

          case 'delete':  // Удаление товара
	          list($product_picture, $product_picture_big, $product_instruct) = 
	            mysql_fetch_row( mysql_query( "select product_picture, product_picture_big, product_instruct 
	                                           from products where product_id = '$id'" ) );
	          if ( $product_picture && file_exists( $_SERVER['DOCUMENT_ROOT'].$product_images.$product_picture ) )
	             unlink($_SERVER['DOCUMENT_ROOT'].$product_images.$product_picture);
	          if ( $product_picture && file_exists( $_SERVER['DOCUMENT_ROOT'].$product_preview_images.$product_picture ) )
	             unlink($_SERVER['DOCUMENT_ROOT'].$product_preview_images.$product_picture);
	          if ( $product_picture_big && file_exists( $_SERVER['DOCUMENT_ROOT'].$product_big_images.$product_picture_big ) )
	             unlink($_SERVER['DOCUMENT_ROOT'].$product_big_images.$product_picture_big);
	          if ( $product_instruct && file_exists( $_SERVER['DOCUMENT_ROOT'].$product_instructions.$product_instruct ) )
	             unlink($_SERVER['DOCUMENT_ROOT'].$product_instructions.$product_instruct);              

              // Удаление дополнительных картинок
              $pictures_sql = mysql_query("select picture_name from product_pictures where product_id = '$id'");
		    
              for ( $i = 0; $i < mysql_num_rows($pictures_sql); $i++ )
              {
                list($picture_name) = mysql_fetch_array($pictures_sql);
                                
                if ( $picture_name && file_exists( $_SERVER['DOCUMENT_ROOT'].$product_additional_images.$picture_name ) )
                  unlink($_SERVER['DOCUMENT_ROOT'].$product_additional_images.$picture_name);
              }
	          
              mysql_query("delete from product_pictures where product_id = '$id'");    	
              
			  mysql_query("delete from products where product_id = '$id'");
            break;
            
          case 'hide':  // Скрытие категории товаров
			list($active) = mysql_fetch_row(mysql_query("select product_active from products where product_id = '$id'"));
			mysql_query("update products set product_active = '".( ($active)?"0":"1" )."' where product_id = '$id'");
            break;
            
          case 'addsave': // Сохранение добавленного товара
          case 'editsave': // Сохранение существующего товара
            $new_name = init_string('new_name',''); $new_name = norm_string($new_name);
            $new_category = init_integer('new_category',0);
            $new_brand = init_integer('new_brand',0);
            $new_addon = init_string('new_addon',''); $new_addon = norm_string($new_addon);
            $new_addon = join( ", ", @spliti( '[[:space:]]+', @eregi_replace( '[[:punct:]]', ' ', $new_addon ) ) );

            if ( !$new_category )
              $error = "Нельзя оставлять товар без категории!";
            else if ( !$new_brand ) 
              $error = "Нельзя оставлять товар без бренда!";
            else
            {
              $new_price = init_float('new_price', -1);

                $new_short_desc = init_string('new_short_desc',''); $new_short_desc = norm_string($new_short_desc);
                $new_full_desc = init_string('new_full_desc',''); $new_full_desc = norm_html($new_full_desc);
                $new_video = init_string('new_video',''); $new_video = norm_html($new_video);
                $new_search = norm_search($new_name).' '.norm_search($new_short_desc).' '.norm_search($new_full_desc);

                if ( $new_name ) 
                {
                  if ( $action == 'editsave' )	
                  {
	                  mysql_query("update products set product_name = '$new_name',
	          	                                       category_id = '$new_category',
	          	                                       brand_id = '$new_brand',
	          	                                       product_price = '$new_price',
	                                                   product_short_desc = '$new_short_desc',
	                                                   product_full_desc = '$new_full_desc',
	                                                   product_video = '$new_video',
	                      							   product_search = '$new_search',	
	                                                   product_addon = '$new_addon'
    	                           where product_id = '$id'");
	              }
	              else
    	          {
	                  mysql_query("insert into products (product_name, category_id, brand_id, product_price, product_picture, product_picture_big,
	                  	  								 product_short_desc, product_full_desc, product_video, product_search, product_addon)
	                                             values ('$new_name', '$new_category', '$new_brand', '$new_price', '', '',
	                  									 '$new_short_desc', '$new_full_desc', '$new_video', '$new_search', '$new_addon')");
    	              $id = mysql_insert_id();

                      $action = 'edit';
	              }
              
                  // Закачка файла с инструкцией
                  if ( $HTTP_POST_FILES['instruct']['error'] != UPLOAD_ERR_NO_FILE )
                  {
                    $file = $HTTP_POST_FILES['instruct']['tmp_name'];
                    $user = $HTTP_POST_FILES['instruct']['name'];
                    $size = $HTTP_POST_FILES['instruct']['size'];
                    $type = $HTTP_POST_FILES['instruct']['type'];
                
                    $tfile = 'i'.$id.strrchr($user, '.'); $tpath = $_SERVER['DOCUMENT_ROOT'].$product_instructions;
                
                    if ( @move_uploaded_file($file,$tpath.$tfile) )
                    {
                      chmod($tpath.$tfile, 0644);
                      mysql_query("update products set product_instruct = '$tfile' where product_id = '$id'");
                    }
                    else $error = "Неизвестная ошибка при закачке файла $user!";
                  }

                  // Закачка файла с большой фотографией
                  if ( $HTTP_POST_FILES['picture_big']['error'] != UPLOAD_ERR_NO_FILE )
                  {
                    $file = $HTTP_POST_FILES['picture_big']['tmp_name'];
                    $user = $HTTP_POST_FILES['picture_big']['name'];
                    $size = $HTTP_POST_FILES['picture_big']['size'];
                    $type = $HTTP_POST_FILES['picture_big']['type'];
                
                    $tfile = 'p'.$id.strrchr($user, '.'); 
                    $tpath = $_SERVER['DOCUMENT_ROOT'].$product_big_images;
                    $tnpath = $_SERVER['DOCUMENT_ROOT'].$product_images;
                    $tprpath = $_SERVER['DOCUMENT_ROOT'].$product_preview_images;
                
                    if ( $type == 'image/jpeg' || $type == 'image/pjpeg' || $type == 'image/gif' || $type == 'image/x-png' || $type == 'image/png' )
                    {
                      if ( @move_uploaded_file($file,$tpath.$tfile) )
                      {
                        ResizeProductImage($tpath.$tfile, $tpath.$tfile, 1000, 1000);
                        chmod($tpath.$tfile, 0644);
                        ResizeProductImage($tpath.$tfile, $tnpath.$tfile, 400, 400);
                        chmod($tnpath.$tfile, 0666);
                        ResizeProductImage($tpath.$tfile, $tprpath.$tfile, 140, 140);
                        chmod($tprpath.$tfile, 0666);
                        
                        mysql_query("update products set product_picture_big = '$tfile',  product_picture = '$tfile' where product_id = '$id'");
                      }
                      else $error = "Неизвестная ошибка при закачке файла $user!";
                    }
                    else $error = "Ошибка! Недопустимый тип файла $user!";
                  }

                }
                else $error = "Нельзя оставлять товар без названия!";
              }

          case 'add':  // Добавление товара
          case 'edit':  // Редактирование товара
            if ( $action == 'editsave' ) $action = 'edit';
            if ( $action == 'addsave' ) $action = 'add';
            
          	if ( $action == 'edit' )
            {
	            $product_sql = mysql_query("select * from products where product_id = '$id'");
	            $product = mysql_fetch_array($product_sql);
	            
	            $product_id = $product['product_id'];
	            $product_name = $product['product_name'];
	            $category_id = $product['category_id'];
	            $brand_id = $product['brand_id'];
	            $product_price = (float) $product['product_price'];
	            $product_short_desc = $product['product_short_desc'];
	            $product_full_desc = htmlspecialchars( $product['product_full_desc'] );
	            $product_video = htmlspecialchars( $product['product_video'] );
	            $product_picture = $product['product_picture'];
	            $product_picture_big = $product['product_picture_big'];
	            $product_instruct = $product['product_instruct'];
				$product_addon = $product['product_addon'];
	        }
	        else
	        {
	            $product_id = $brand_id = $category_id = $product_price = 0;
	            $product_name = $product_short_desc = $product_full_desc = $product_video = $product_picture = $product_picture_big = $product_instruct = $product_addon = '';
	        }
	        
            $category_select_str = $category_insert_select_str = '';
            
            if ( $action == 'add' )
            {
	          $category_select_str .= "<select name=\"new_category\" style=\"width: 300px\" onchange=\"form.new_name.value = form.new_insert_category.options[form.new_category.selectedIndex].text + ' ' + form.new_brand.options[form.new_brand.selectedIndex].text\">";
              $category_select_str .= "<option value=\"0\" selected=\"selected\"></option>";

	          $category_insert_select_str .= "<select name=\"new_insert_category\" style=\"display: none\">";
              $category_insert_select_str .= "<option value=\"0\" selected=\"selected\"></option>";
            }
            else
              $category_select_str = "<select name=\"new_category\" style=\"width: 300px\">";

            $category_sql = mysql_query("select category_id, category_name, category_insert_name from categories order by category_name");
            
            while ( $category_select = mysql_fetch_array($category_sql) )
            {
              $category_select_id = $category_select['category_id'];
              $category_select_name = $category_select['category_name'];
              $category_insert_select_name = $category_select['category_insert_name'];
              
              $category_select_str .= "<option value=\"$category_select_id\"".( ($category_select_id == $category_id)?" selected=\"selected\"":"" ).">$category_select_name</option>";
              
              if ( $action == 'add' )
                $category_insert_select_str .= "<option value=\"$category_select_id\">$category_insert_select_name</option>";
            }
            $category_select_str .= "</select>\n";
            $category_insert_select_str .= "</select>\n";

            $brand_select_str = '';
            
            if ( $action == 'add' )
            {
              $brand_select_str .= "<select name=\"new_brand\" style=\"width: 300px\" onchange=\"form.new_name.value = form.new_insert_category.options[form.new_category.selectedIndex].text + ' ' + form.new_brand.options[form.new_brand.selectedIndex].text\">";
	          $brand_select_str .= "<option value=\"0\" selected=\"selected\"></option>";
            }
            else
	          $brand_select_str .= "<select name=\"new_brand\" style=\"width: 300px\">";
	        
            $brand_sql = mysql_query("select brand_id, brand_name from brands order by brand_name");
            while ( $brand_select = mysql_fetch_array($brand_sql) )
            {
              $brand_select_id =  $brand_select['brand_id'];
              $brand_select_name =  $brand_select['brand_name'];
              $brand_select_str .= "<option value=\"$brand_select_id\"".( ($brand_select_id == $brand_id)?" selected=\"selected\"":"" ).">$brand_select_name</option>";
            }
            $brand_select_str .= "</select>\n";
            
            if ( !file_exists($_SERVER['DOCUMENT_ROOT'].$product_preview_images.$product_picture) ||
            	 !is_file($_SERVER['DOCUMENT_ROOT'].$product_preview_images.$product_picture) )
              $product_picture = $product_preview_picture_default;
            
			if ( !file_exists($_SERVER['DOCUMENT_ROOT'].$product_big_images.$product_picture_big) ||
				 !is_file($_SERVER['DOCUMENT_ROOT'].$product_big_images.$product_picture_big) )
			  $product_picture_big = '';

            if ( $action == 'edit' )
            {
              $category = $category_id;
              list($group) = mysql_fetch_row(mysql_query("select group_id from categories where category_id = '$category'"));
            }

            list($group_name) = mysql_fetch_row(mysql_query("select group_name from groups where group_id = '$group'"));
            list($category_name) = mysql_fetch_row(mysql_query("select category_name from categories where category_id = '$category'"));

           	PrintHeader( "<p><a href=\"index.php?tab=group\">Группы товаров</a> - <a href=\"index.php?tab=category&group=$group\">$group_name</a> - <a href=\"index.php?tab=product&group=$group&category=$category\">$category_name</a> - ".( ( $action == 'edit' )?$product_name:"Добавление товара" )."</p>" );

            if ( $error ) { ?>    	  
    <p class="baseerror"><?= $error ?></p>
<?          } ?>
    <form id="form" name="form" method="post" enctype="multipart/form-data" onsubmit="return (form.new_name.value.length>0)">
      <input type="hidden" name="id" value="<?= $id ?>"/>
      <input type="hidden" name="action" value="<?= ( $action == 'edit' )?'editsave':'addsave' ?>"/>
      <input type="hidden" name="tab" value="product"/>
      <input type="hidden" name="group" value="<?= $group ?>"/>
      <input type="hidden" name="category" value="<?= $category ?>"/>
      <table width="100%" cellspacing="0" cellpadding="2" border="0" style="margin-top: 10px">
        <tr>
          <td valign="top" style="width: 120px">
            <p>Название</p>
          </td>
          <td valign="top" colspan="2">
            <input type="text" class="text" name="new_name" style="width: 700px" value="<?= $product_name ?>"/>
          </td>
          <td rowspan="12" align="center" valign="top">
            <img src="<?= $product_preview_images.$product_picture ?>" alt="<?= $product_name ?>"/>
<?		if ( $product_picture_big ) { ?>
			<br/><a href="<?= $product_big_images.$product_picture_big ?>">Фотография</a>
<?		} ?>
          </td>
        </tr>
        <tr>
          <td valign="top">
            <p>Категория</p>
          </td>
          <td valign="top" style="width: 300px">
            <?= $category_select_str ?>
<?          if ( $action == 'add' ) { ?>
            <?= $category_insert_select_str ?>
<?          } ?>
          </td>
          <td valign="top" style="width: 200px" align="right">
            <input type="submit" class="button" style="width: 120px" value="Сохранить"/>
          </td>
        </tr>
        <tr>
          <td valign="top">
            <p>Бренд</p>
          </td>
          <td valign="top" colspan="2">
            <?= $brand_select_str ?>
          </td>
        </tr>
        <tr>
          <td valign="top">
            <p>Цена</p>
          </td>
          <td valign="top" colspan="2">
            <p><input type="text" class="text" name="new_price" style="width: 100px; text-align: right" value="<?= $product_price ?>"/></p>
          </td>
        </tr>
        <tr>
          <td valign="top">
            <p>Фотография</p>
          </td>
          <td valign="top" colspan="2">
            <input class="text" type="file" name="picture_big" style="width: 300px; border: none;" size="44"/>
          </td>
        </tr>
        <tr>
          <td valign="top">
            <p>Инструкция</p>
          </td>
          <td valign="top">
            <p><input class="text" type="file" name="instruct" style="width: 300px; border: none;" size="44"/></p>
          </td>
          <td valign="top" align="center">
<?          if ( $product_instruct && file_exists($_SERVER['DOCUMENT_ROOT'].$product_instructions.$product_instruct) ) { ?>
            <p><a href="<?= $product_instructions.$product_instruct ?>" target="_blank">инструкция</a></p>
<?          } else { ?>
            <p>инструкция отсутствует</p>
<?          } ?>
          </td>
        </tr>
        <tr>
          <td valign="top">
            <p>Аксессуары</p>
          </td>
          <td valign="top" colspan="2">
            <input type="text" class="text" name="new_addon" style="width: 700px" value="<?= $product_addon ?>"/>
          </td>
        </tr>
        <tr>
          <td valign="top">
            <p>Кр. описание</p>
          </td>
          <td valign="top" colspan="2">
            <textarea name="new_short_desc" cols="70" rows="3" style="width: 700px"><?= $product_short_desc ?></textarea>
          </td>
        </tr>
        <tr>
          <td valign="top">
            <p>Подр. описание</p>
          </td>
          <td valign="top" colspan="2">
            <textarea name="new_full_desc" cols="70" rows="15" class="format" style="width: 700px"><?= $product_full_desc ?></textarea>
          </td>
        </tr>
        <tr>
          <td valign="top">
            <p>Видео</p>
          </td>
          <td valign="top" colspan="2">
            <textarea name="new_video" cols="70" rows="5" class="format" style="width: 700px"><?= $product_video ?></textarea>
          </td>
        </tr>
        <tr>
          <td valign="top">
            &nbsp;
          </td>
          <td valign="top" colspan="2">
<?          if ( $action == 'edit' ) { ?>
            <table width="100%" cellspacing="0" cellpadding="0" border="0">
              <tr>
                <td width="50%">
                  <p><a href="index.php?tab=picture&id=<?= $id ?>&group=<?= $group ?>&category=<?= $category ?>">Дополнительные фотографии</a></p>
                </td>
                <td width="50%">
                  &nbsp;
                </td>
              </tr>
            </table>
<?          } else { ?>
            &nbsp;
<?          } ?>
          </td>
        </tr>
      </table>
    </form>
  </body>
</html>
<?    
            exit;
    	}

        $products_cnt_sql = mysql_query("select count(*) as products_cnt
  		                                 from products
  		                                 where products.category_id = '$category' ".( ($active_only == 'true')?"and products.product_active = '1'":"" ));
        
        list($products_cnt) = mysql_fetch_row($products_cnt_sql);
        
        if ( $page * $max_product_list >= $products_cnt )
        	$page = floor( ($products_cnt - 1) / $max_product_list );
        if ( $page < 0 ) $page = 0;

        $link_str = '';         
        if ( $products_cnt >= $max_product_list )
        {
          $qs = @ereg_replace("&page=[0-9]+","",$_SERVER["QUERY_STRING"]);
          for ( $p = 0; $p < $products_cnt / $max_product_list ; $p++ )
          {
            if ( $p == $page )
              $link_str .= "<b>".($p + 1)."</b>&nbsp;&nbsp;";
            else
              $link_str .= "<a href=\"index.php?$qs&page=$p\">".($p + 1)."</a>&nbsp;&nbsp;";
          }          
          $link_str = "Страницы:&nbsp;&nbsp;".substr($link_str, 0, strlen($link_str) - 12);
        }       

        list($group_name) = mysql_fetch_row(mysql_query("select group_name from groups where group_id = '$group'"));
        list($category_name, $category_by_course) = mysql_fetch_row(mysql_query("select category_name, category_by_course from categories where category_id = '$category'"));

        PrintHeader( "<p><a href=\"index.php?tab=group\">Группы товаров</a> - <a href=\"index.php?tab=category&group=$group\">$group_name</a> - $category_name</p>" );
 
    if ( $error ) { ?>
    <p class="baseerror"><?= $error ?></p>
<?  } ?>
    <form id="form" name="form" method="post">
      <input type="hidden" name="action" value="<? if ( $action == 'editprice' ) print 'saveprice' ?>"/>
      <input type="hidden" name="tab" value="product"/>
      <input type="hidden" name="id" value="<? if ( $action == 'editprice' ) print $id ?>"/>
      <input type="hidden" name="page" value="<?= $page ?>"/>
      <input type="hidden" name="sort" value="<?= $sort ?>"/>
      <input type="hidden" name="order" value="<?= $order ?>"/>
      <input type="hidden" name="group" value="<?= $group ?>"/>
      <input type="hidden" name="category" value="<?= $category ?>"/>
      <table width="100%" cellspacing="0" cellpadding="1" border="0" style="margin-top: 10px; border-collapse: collapse">
        <tr style="height: 26px">
          <td width="30">
            &nbsp;
          </td>
          <td width="30">
            &nbsp;
          </td>
          <td width="45" align="center" style="border: 1px solid #92BA85; background-color: #FFFFFF">
            <p class="link"><a href="index.php?tab=product&group=<?= $group ?>&category=<?= $category ?>&sort=id&order=<?= ( $sort == 'id' )?$next_order:'asc' ?>&page=0">ID</a><? if ( $sort == 'id' ) print " <img src=\"../images/admin/".$order.".gif\"/>" ?></p>
          </td>
          <td align="center" style="border: 1px solid #92BA85; border-left: none; background-color: #FFFFFF">
            <p class="link"><a href="index.php?tab=product&group=<?= $group ?>&category=<?= $category ?>&sort=name&order=<?= ( $sort == 'name' )?$next_order:'asc' ?>&page=0">Наименование товара</a><? if ( $sort == 'name' ) print " <img src=\"../images/admin/".$order.".gif\"/>" ?></p>
          </td>
          <td width="150" align="center" style="border: 1px solid #92BA85; background-color: #FFFFFF" colspan="2">
            <p class="link"><a href="index.php?tab=product&group=<?= $group ?>&category=<?= $category ?>&sort=price&order=<?= ( $sort == 'price' )?$next_order:'asc' ?>&page=0">Цена</a><? if ( $sort == 'price' ) print " <img src=\"../images/admin/".$order.".gif\"/>" ?></p>
          </td>
        </tr>
<?    
		switch ( $sort )
		{
          case 'name': $sort = 'products.product_name'; break;
          case 'price': $sort = 'products.product_price'; break;
          default: $sort = 'products.product_id';
		}
				
	    $product_start_list = $page * $max_product_list;
        $products_sql = mysql_query("select products.product_id, products.product_name, products.product_active, products.product_price
  		                             from products
                                     where products.category_id = '$category' ".( ($active_only == 'true')?"and products.product_active = '1'":"" )."
                                     group by products.product_id
                                     order by $sort $order
                                     limit $product_start_list, $max_product_list");

        for ( $i = 0; $i < mysql_num_rows($products_sql); $i++ )
        {
          $products = mysql_fetch_array($products_sql);
    
          $product_id = $products['product_id'];
          $product_name = $products['product_name'];
          $product_active = $products['product_active'];
          $product_price = (float) $products['product_price'];
		  $product_price = round($category_by_course ? $product_price * $course : $product_price);
?>
        <tr style="height: 26px">
          <td align="center">
            <input type="button" class="ibutton" value="" style="background-image: url(../images/admin/buttonminus.gif);" onclick="if (confirm('Удалить \'<?= str_replace("\"","&quot;", str_replace("'","\\'",$product_name)) ?>\'?')) { action.value='delete'; form.id.value='<?= $product_id ?>'; submit(); }" title="Удалить"/>
          </td>
          <td align="center">
            <input type="button" class="ibutton" value="" style="background-image: url(../images/admin/buttonhide.gif);" onclick="form.action.value='hide'; form.id.value='<?= $product_id ?>'; submit();" title="<?= ($product_active)?"Скрыть":"Показать" ?>"/>
          </td>
          <td align="right" style="border: 1px solid #92BA85; background-color: <?= ($i % 2)?"#FFFFFF":"#F8FCF8" ?>">
            <p><?= $product_id ?>.&nbsp;</p>
          </td>
          <td style="border: 1px solid #92BA85; background-color: <?= ($i % 2)?"#FFFFFF":"#F8FCF8" ?>">
            <p>&nbsp;<a href="index.php?tab=product&action=edit&group=<?= $group ?>&category=<?= $category ?>&id=<?= $product_id ?>"<? if (!$product_active) print " class=\"hide\"" ?>><?= $product_name ?></a></p>
          </td>
<?  if ( $action == 'editprice' && $product_id == $id ) { ?>
          <td align="right" style="width: 75px; border: 1px solid #92BA85; background-color: <?= ($i % 2)?"#FFFFFF":"#F8FCF8" ?>">
            <input type="text" class="text" name="new_price" style="width: 75px; text-align: right" value="<?= $product_price ?>" onchange="form.action.value='saveprice'; form.id.value='<?= $product_id ?>';"/>
          </td>
<?  } else { ?>
          <td align="center" style="width: 75px; border: 1px solid #92BA85; background-color: <?= ($i % 2)?"#FFFFFF":"#F8FCF8" ?>">
            <p style="cursor: default" onclick="form.action.value='editprice'; form.id.value='<?= $product_id ?>'; form.submit();"><?= $product_price ?></p>
          </td>
<?  } ?>
          <td align="center" style="width: 75px; border: 1px solid #92BA85; background-color: <?= ($i % 2)?"#FFFFFF":"#F8FCF8" ?>">
            <p><?= $product_price ?></p>
          </td>
        </tr>
<?   
  }
?>
      </table>
      <table width="100%" cellspacing="0" cellpadding="1" border="0" style="border-collapse: collapse" >
        <tr style="height: 26px">
          <td width="30">
            &nbsp;
          </td>
          <td width="30" align="center">
            <input type="button" class="ibutton" value="" style="background-image: url(../images/admin/buttonplus.gif);" onclick="form.action.value='add'; submit();" title="Добавить"/>
          </td>
          <td width="45">
            &nbsp;
          </td>
          <td align="left">
            <p>&nbsp;<?= $link_str ?></p>
          </td>
          <td width="220" align="center">
            <p><input type="checkbox" name="active_only" value="true" onclick="form.action.value=''; submit();"<?= ( $active_only == 'true' )?" checked=\"checked\"":"" ?>/> Не показывать скрытые</p>
          </td>
        </tr>
      </table>
    </form>
<?
  
  function ResizeProductImage($original_picture, $preview_picture, $maxwidth = 140, $maxheight = 140)
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