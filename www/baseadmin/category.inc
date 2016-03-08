<?
        $id = init_integer('id',0);
        $group = init_integer('group',0);
        
        $error = '';

        list($group_name) = mysql_fetch_row(mysql_query("select group_name from groups where group_id = '$group'"));

        switch ( $action )
        {
          case 'moveup':  // Перемещение категории товаров
            list($order) = mysql_fetch_row(mysql_query("select category_order from categories where category_id = '$id'"));
            list($new_id, $new_order) = mysql_fetch_row(mysql_query("select category_id, category_order from categories 
	                                                                 where category_order < '$order' 
	                                                                   and group_id = '$group' 
	                                                                 order by category_order desc limit 1"));
            if ( $new_id && $new_order )
            {
              mysql_query("update categories set category_order = '$new_order' where category_id = '$id'");
              mysql_query("update categories set category_order = '$order' where category_id = '$new_id'");
            }
    	    break;

          case 'delete':  // Удаление категории товаров
            list($cnt) = mysql_fetch_row(mysql_query("select count(*) from products where category_id = '$id'"));
            if (!$cnt) 
            {
              list($category_picture) = 
              	mysql_fetch_row(mysql_query("select category_picture
                                             from categories where category_id = '$id'"));
              if ( $category_picture && file_exists( $_SERVER['DOCUMENT_ROOT'].$category_images.$category_picture ) )
              unlink($_SERVER['DOCUMENT_ROOT'].$category_images.$category_picture);

              mysql_query("delete from categories where category_id = '$id'");
            }
            else $error = "Нельзя удалить непустую категорию!";
            break;
          
          case 'hide':  // Скрытие категории товаров
            list($active) = mysql_fetch_row(mysql_query("select category_active from categories where category_id = '$id'"));
            mysql_query("update categories set category_active = '".( ($active)?"0":"1" )."' where category_id = '$id'");
            break;
            
          case 'addsave':  // Сохранение добавленной категории
          case 'editsave': // Редактирование категории товаров
            $new_name = init_string('new_name',''); $new_name = norm_string($new_name); 
            $new_short_name = init_string('new_short_name',''); $new_short_name = norm_string($new_short_name); 
            $new_insert_name = init_string('new_insert_name',''); $new_insert_name = norm_string($new_insert_name); 
            $new_comment = init_string('new_comment',''); $new_comment = norm_string($new_comment);
            $new_by_course = init_string('new_by_course') ? 1 : 0;
            
            if ( !$new_name )
              $error = "Нельзя оставлять категорию без названия!";
            else
            {
              if ( $action == 'editsave' )	
              {
                mysql_query("update categories set category_name = '$new_name',         	                                       
                	                               category_short_name = '$new_short_name',
                	                               category_insert_name = '$new_insert_name',
                                                   category_comment = '$new_comment',
                                                   category_by_course = '$new_by_course'
                             where category_id = '$id'");
              }
              else
              {
                list($order) = mysql_fetch_row(mysql_query("select category_order from categories 
                                                            where group_id = '$group' 
                                                            order by category_order desc limit 1"));
                $order++;
                mysql_query("insert into categories (group_id, category_name, category_short_name, category_insert_name, category_comment, category_by_course, category_order)
                                              values('$group', '$new_name', '$new_short_name', '$new_insert_name', '$new_comment', '$new_by_course', '$order')");
                $id = mysql_insert_id();

                $action = 'edit';
              }
              
              if ( $_FILES['file']['error'] != UPLOAD_ERR_NO_FILE )
              {
                $file = $_FILES['file']['tmp_name'];
                $user = $_FILES['file']['name'];
                $size = $_FILES['file']['size'];
                $type = $_FILES['file']['type'];

                $tfile = 'c'.$id.strrchr($user, '.'); $tpath = $_SERVER['DOCUMENT_ROOT'].$category_images;
                    
                if ( $type == 'image/jpeg' || $type == 'image/pjpeg' || $type == 'image/gif' || $type == 'image/x-png' || $type == 'image/png' )
                {
                  if ( @move_uploaded_file($file, $tpath.$tfile) )
                  {
                    chmod($tpath.$tfile, 0644);
                    mysql_query("update categories set category_picture = '$tfile' where category_id = '$id'");
                  }
                  else $error = "Неизвестная ошибка при закачке файла $user!";
                }
                else $error = "Ошибка! Недопустимый тип файла $user!";
              }
            }
 
          case 'add':   // Добавление категории товаров
          case 'edit':  // Редактирование категории товаров
            if ( $action == 'addsave' ) $action = 'add';          	  
            if ( $action == 'editsave' ) $action = 'edit';
          	  
            if ( $action == 'edit' )
            {
              $category_sql = mysql_query("select * from categories where category_id = '$id'");
              $category = mysql_fetch_array($category_sql);
            
              $category_id = $category['category_id'];
              $category_name = $category['category_name'];
              $category_short_name = $category['category_short_name'];
              $category_insert_name = $category['category_insert_name'];
              $category_picture = $category['category_picture'];
              $category_comment = $category['category_comment'];
              $category_by_course = $category['category_by_course'];
            }
            else
            {
              $category_id = 0;
              $category_name = $category_short_name = $category_insert_name = $category_picture = $category_comment = $category_by_course = '';
            }
            
            if ( !file_exists($_SERVER['DOCUMENT_ROOT'].$category_images.$category_picture) ||
            	 !is_file($_SERVER['DOCUMENT_ROOT'].$category_images.$category_picture) )
              $category_picture = $category_picture_default;
            
           	PrintHeader( "<p><a href=\"index.php?tab=group\">Группы товаров</a> - <a href=\"index.php?tab=category&group=$group\">$group_name</a> - ".( ( $action == 'edit' )?$category_name:"Добавление категории" )."</p>" );

            if ( $error ) { ?>
    <p class="baseerror"><?= $error ?></p>
<?          } ?>
    <form id="form" name="form" method="post" enctype="multipart/form-data" onsubmit="return (form.new_name.value.length>0)">
      <input type="hidden" name="id" value="<?= $id ?>"/>
      <input type="hidden" name="action" value="<?= ( $action == 'edit' )?'editsave':'addsave' ?>"/>
      <input type="hidden" name="tab" value="category"/>
      <input type="hidden" name="group" value="<?= $group ?>"/>
      <table width="100%" cellspacing="0" cellpadding="2" border="0" style="margin-top: 10px">
        <tr>
          <td valign="top" style="width: 100px">
            <p>Название</p>
          </td>
          <td valign="top" style="width: 300px">
            <input type="text" class="text" name="new_name" style="width: 300px" value="<?= $category_name ?>"/>
          </td>
          <td valign="top" style="width: 200px" align="right">
            <input type="submit" class="button" style="width: 120px" value="Сохранить"/>
          </td>
          <td rowspan="5" align="center" valign="top">
            <img src="<?= $category_images.$category_picture ?>" alt="<?= $category_name ?>"/>
          </td>
        </tr>
        <tr>
          <td valign="top">
            <p>Кр. название</p>
          </td>
          <td valign="top" colspan="2">
            <input type="text" class="text" name="new_short_name" style="width: 300px" value="<?= $category_short_name ?>"/>
          </td>
        </tr>
        <tr>
          <td valign="top">
            <p>Вставка</p>
          </td>
          <td valign="top" colspan="2">
            <input type="text" class="text" name="new_insert_name" style="width: 300px" value="<?= $category_insert_name ?>"/>
          </td>
        </tr>
        <tr>
          <td valign="top">
            <p>Пересчитывать<br/> в доллары</p>
          </td>
          <td valign="top" colspan="2">
            <input type="checkbox" name="new_by_course" value="1"<? if ($category_by_course) { ?> checked="checked"<? } ?>/>
          </td>
        </tr>
        <tr>
          <td valign="top">
            <p>Фотография</p>
          </td>
          <td valign="top" colspan="2">
            <input class="text" type="file" name="file" style="width: 300px; border: none;" size="44"/>
          </td>
        </tr>
        <tr>
          <td valign="top">
            <p>Комментарий</p>
          </td>
          <td valign="top" colspan="2">
            <textarea name="new_comment" cols="50" rows="3" style="width: 500px"><?= $category_comment ?></textarea>
          </td>
        </tr>
      </table>
    </form>
  </body>
</html>
<?    
          exit;
    	}
    	
       	PrintHeader( "<p><a href=\"index.php?tab=group\">Группы товаров</a> - $group_name</p>" );

        if ( $error ) { ?>    	  
    <p class="baseerror"><?= $error ?></p>
<?  } ?>
    <form id="form" name="form" method="post">
      <input type="hidden" name="id" value=""/>
      <input type="hidden" name="action" value=""/>
      <input type="hidden" name="tab" value="category"/>
      <input type="hidden" name="group" value="<?= $group ?>"/>
      <table width="100%" cellspacing="0" cellpadding="0" border="0" style="margin-top: 10px">
<?    
  $categories_sql = mysql_query("select categories.category_id, categories.category_name,
  		                                categories.category_order,categories.category_active,
  		                                count(products.product_id) as products_cnt
  		                         from categories left join products
  		                         on categories.category_id = products.category_id
                                 where categories.group_id = '$group' 
                                 group by categories.category_id
                                 order by categories.category_order");

  for ( $i = 0; $i < mysql_num_rows($categories_sql); $i++ )
  {
    $categories = mysql_fetch_array($categories_sql);
    
  	$category_id = $categories['category_id'];
  	$category_name = $categories['category_name'];
  	$category_order = $categories['category_order'];
  	$category_active = $categories['category_active'];
  	$products_cnt = $categories['products_cnt'];

?>
        <tr style="height: 28px">
          <td width="30" align="center">
<?  if ( !$products_cnt ) { ?>
            <input type="button" class="ibutton" value="" style="background-image: url(../images/admin/buttonminus.gif);" onclick="if (confirm('Удалить категорию товаров \'<?= str_replace("\"","&quot;", str_replace("'","\\'",$category_name)) ?>\'?')) { action.value='delete'; form.id.value='<?= $category_id ?>'; submit(); }" title="Удалить"/>
<?  } else { ?>
            &nbsp;
<?  } ?>
          </td>
          <td width="30" align="center">
<?  if ( $i ) { ?>
            <input type="button" class="ibutton" value="" style="background-image: url(../images/admin/buttonup.gif);" onclick="form.action.value='moveup'; form.id.value='<?= $category_id ?>'; submit();" title="Переместить наверх"/>
<?  } else { ?>
            &nbsp;
<?  } ?>
          </td>
          <td width="30" align="center">
            <input type="button" class="ibutton" value="" style="background-image: url(../images/admin/buttonhide.gif);" onclick="form.action.value='hide'; form.id.value='<?= $category_id ?>'; submit();" title="<?= ($category_active)?"Скрыть":"Показать" ?>"/>
          </td>
          <td width="30" align="center">
            <input type="button" class="ibutton" value="" style="background-image: url(../images/admin/buttonedit.gif);" onclick="form.action.value='edit'; form.id.value='<?= $category_id ?>'; submit();" title="Редактировать"/>
          </td>
          <td>
            <p><a href="index.php?tab=product&group=<?= $group ?>&category=<?= $category_id ?>"<? if (!$category_active) print " class=\"hide\"" ?>><?= $category_name ?></a></p>
          </td>
        </tr>
<?    
  }
?>
        <tr style="height: 28px">
          <td width="30">
            &nbsp;
          </td>
          <td width="30">
            &nbsp;
          </td>
          <td width="30">
            &nbsp;
          </td>
          <td width="30" align="center">
            <input type="button" class="ibutton" value="" style="background-image: url(../images/admin/buttonplus.gif);" onclick="form.action.value='add'; submit();" title="Добавить"/>
          </td>
          <td>
            &nbsp;
          </td>
        </tr>
      </table>
    </form>
