<?
        $id = init_integer('id',0);
        
      	$tab = 'group';

        $error = '';

        switch ( $action )
        {
          case 'moveup':  // Перемещение группы товаров
              list($order) = mysql_fetch_row(mysql_query("select group_order from groups where group_id = '$id'"));
              list($new_id, $new_order) = mysql_fetch_row(mysql_query("select group_id, group_order from groups 
			                                                           where group_order < '$order' 
			                                                           order by group_order desc limit 1"));			
              if ( $new_id && $new_order )
			  {
                mysql_query("update groups set group_order = '$new_order' where group_id = '$id'");
                mysql_query("update groups set group_order = '$order' where group_id = '$new_id'");
			  }
    	    break;

          case 'delete':  // Удаление группы товаров
              list($cnt) = mysql_fetch_row(mysql_query("select count(*) from categories where group_id = '$id'"));
			  if (!$cnt) mysql_query("delete from groups where group_id = '$id'");
              else $error = "Нельзя удалить непустую группу!";
            break;
          
          case 'hide':  // Скрытие группы товаров
              list($active) = mysql_fetch_row(mysql_query("select group_active from groups where group_id = '$id'"));
			  mysql_query("update groups set group_active = '".( ($active)?"0":"1" )."' where group_id = '$id'");
            break;

          case 'addsave':  // Дабавление группы товаров
          case 'editsave': // Редактирование группы товаров
            $new_name = init_string('new_name',''); $new_name = norm_string($new_name);
            $new_comment = init_string('new_comment',''); $new_comment = norm_string($new_comment);
          	  
            if ( $new_name ) 
            {
              if ( $action == 'editsave' )	
              {
                mysql_query("update groups set group_name = '$new_name',
                                               group_comment = '$new_comment'
                             where group_id = '$id'");
              }
              else
              {
                list($order) = mysql_fetch_row(mysql_query("select group_order from groups 
                                                            order by group_order desc limit 1"));
                $order++;
                mysql_query("insert into groups (group_name, group_comment, group_order)
                                          values('$new_name', '$new_comment', '$order')");
                $id = mysql_insert_id();

                $action = 'edit';
              }
            }
   			else $error = "Нельзя оставлять группу без названия!";
    	    
          case 'add':   // Добавление группы товаров
          case 'edit':  // Редактирование группы товаров
            if ( $action == 'addsave' ) $action = 'add';          	  
            if ( $action == 'editsave' ) $action = 'edit';
          	  
            if ( $action == 'edit' )
            {
              $group_sql = mysql_query("select * from groups where group_id = '$id'");
              $group = mysql_fetch_array($group_sql);
            
              $group_id = $group['group_id'];
              $group_name = $group['group_name'];
              $group_comment = $group['group_comment'];
            }
            else
            {
              $group_id = 0;
              $group_name = $group_comment = '';
            }

           	PrintHeader( "<p><a href=\"index.php?tab=group\">Группы товаров</a> - ".( ( $action == 'edit' )?$group_name:"Добавление группы" )."</p>" );

            if ( $error ) { ?>
    <p class="baseerror"><?= $error ?></p>
<?          } ?>
    <form id="form" name="form" method="post" onsubmit="return (form.new_name.value.length>0)">
      <input type="hidden" name="id" value="<?= $id ?>"/>
      <input type="hidden" name="action" value="<?= ( $action == 'edit' )?'editsave':'addsave' ?>"/>
      <input type="hidden" name="tab" value="group"/>
      <table width="100%" cellspacing="0" cellpadding="2" border="0" style="margin-top: 10px">
        <tr>
          <td valign="top" style="width: 100px">
            <p>Название</p>
          </td>
          <td valign="top" style="width: 300px">
            <input type="text" class="text" name="new_name" style="width: 300px" value="<?= $group_name ?>"/>
          </td>
          <td valign="top" style="width: 200px" align="right">
            <input type="submit" class="button" style="width: 120px" value="Сохранить"/>
          </td>
          <td rowspan="2" align="center" valign="top">
            &nbsp;
          </td>
        </tr>
        <tr>
          <td valign="top">
            <p>Комментарий</p>
          </td>
          <td valign="top" colspan="2">
            <textarea name="new_comment" cols="50" rows="3" style="width: 500px"><?= $group_comment ?></textarea>
          </td>
        </tr>
      </table>
    </form>
  </body>
</html>
<?    
          exit;
    	}
    	
    	PrintHeader( "<p>Группы товаров</p>" );

    	if ( $error ) { ?>    	  
    <p class="baseerror"><?= $error ?></p>
<?      } ?>
    <form id="form" name="form" action="index.php" method="post">
      <input type="hidden" name="id" value=""/>
      <input type="hidden" name="action" value=""/>
      <input type="hidden" name="tab" value="group"/>
      <table width="100%" cellspacing="0" cellpadding="0" border="0" style="margin-top: 10px">
<?    
  $groups_sql = mysql_query("select groups.group_id, groups.group_name,
  		                                groups.group_order, groups.group_active,
  		                                count(categories.category_id) as categories_cnt
  		                         from groups left join categories
  		                         on groups.group_id = categories.group_id
                                 group by groups.group_id
                                 order by groups.group_order");

  for ( $i = 0; $i < mysql_num_rows($groups_sql); $i++ )
  {
    $groups = mysql_fetch_array($groups_sql);
    	
  	$group_id = $groups['group_id'];
  	$group_name = $groups['group_name'];
  	$group_order = $groups['group_order'];
  	$group_active = $groups['group_active'];
  	$categories_cnt = $groups['categories_cnt'];

?>
        <tr style="height: 28px">
          <td width="30" align="center">
<?  if ( !$categories_cnt ) { ?>
            <input type="button" class="ibutton" value="" style="background-image: url(../images/admin/buttonminus.gif);" onclick="if (confirm('Удалить группу товаров \'<?= str_replace("'","\\'",$group_name) ?>\'?')) { action.value='delete'; form.id.value='<?= $group_id ?>'; submit(); }" title="Удалить"/>
<?  } else { ?>
            &nbsp;
<?  } ?>
          </td>
          <td width="30" align="center">
<?  if ( $i ) { ?>
            <input type="button" class="ibutton" value="" style="background-image: url(../images/admin/buttonup.gif);" onclick="form.action.value='moveup'; form.id.value='<?= $group_id ?>'; submit();" title="Переместить наверх"/>
<?  } else { ?>
            &nbsp;
<?  } ?>
          </td>
          <td width="30" align="center">
            <input type="button" class="ibutton" value="" style="background-image: url(../images/admin/buttonhide.gif);" onclick="form.action.value='hide'; form.id.value='<?= $group_id ?>'; submit();" title="<?= ($group_active)?"Скрыть":"Показать" ?>"/>
          </td>
          <td width="30" align="center">
            <input type="button" class="ibutton" value="" style="background-image: url(../images/admin/buttonedit.gif);" onclick="form.action.value='edit'; form.id.value='<?= $group_id ?>'; submit();" title="Редактировать"/>
          </td>
          <td>
<?  if ( $action == 'edit' && $group_id == $id ) { ?>
            <input type="text" class="text" name="new_name" style="width: 200px; height: 17px; margin: 0px" value="<?= $group_name ?>"/>&nbsp;&nbsp;<input type="button" class="ibutton" value="" style="background-image: url(../images/admin/buttonok.gif);" onclick="if ( form.new_name.value.length > 0 ) { form.action.value='editsave'; form.id.value='<?= $group_id ?>'; submit(); }" title="Сохранить"/>
<?  } else { ?>
            <p><a href="index.php?tab=category&group=<?= $group_id ?>"<? if (!$group_active) print " class=\"hide\"" ?>><?= $group_name ?></a></p>
<?  } ?>
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
          <td align="left">
            &nbsp;
          </td>
        </tr>
      </table>
    </form>
