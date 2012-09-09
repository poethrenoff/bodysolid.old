<?
	$link_id = init_integer('link_id', 0);

	$error = '';

	switch ( $action )
	{
		case 'hide':  // Скрытие ссылки
			list($active) = mysql_fetch_row(mysql_query("select link_active from links where link_id = '$link_id'"));
			mysql_query("update links set link_active = '".( ($active)?"0":"1" )."' where link_id = '$link_id'");
			break;

		case 'delete':	// Удаление ссылки
			mysql_query("delete from links where link_id = '$link_id'");
			break;

		case 'addsave':  // Сохранение добавленной ссылки
		case 'editsave': // Сохранение отредактированной ссылки
			$new_url = init_string('new_url',''); $new_url = norm_string($new_url);
			$new_title = init_string('new_title', ''); $new_title = norm_string($new_title);
			$new_text = init_string('new_text',''); $new_text = norm_html($new_text);
			
			if ( !$new_url )
				$error = "Нельзя оставлять ссылку без ссылки!";
			if ( !$new_title )
				$error = "Нельзя оставлять ссылку без заголовка!";
			else
			{
				if ( $action == 'editsave' )	
				{
					mysql_query("update links set link_url = '$new_url',
											      link_title = '$new_title',
											      link_text = '$new_text'
								 where link_id = '$link_id'");
				}
				else
				{
					mysql_query("insert into links (link_url, link_title, link_text)
								 			 values('$new_url', '$new_title', '$new_text')");
					$link_id = mysql_insert_id();
					$action = 'edit';
				}
			}			

		case 'add':   // Добавление ссылки
		case 'edit':  // Редактирование ccылки
			if ( $action == 'addsave' ) $action = 'add';          	  
			if ( $action == 'editsave' ) $action = 'edit';

			if ( $action == 'edit' )
			{
				$links_sql = mysql_query("select * from links where link_id = '$link_id'");
				$links_row = mysql_fetch_array($links_sql);

				$link_url = $links_row['link_url'];
				$link_title = $links_row['link_title'];
				$link_text = $links_row['link_text'];
			}
			else
			{
				$link_url = 'http://';
				$link_title = $link_text = '';
			}

            if ( $error ) { ?>
		<p class="baseerror"><?= $error ?></p> 
<?			} ?>
		<form id="form" name="form" method="post" onsubmit="return ( form.new_title.value.length > 0 && form.new_url.value.length > 0 )">
			<input type="hidden" name="tab" value="links"/>
			<input type="hidden" name="action" value="<?= ( $action == 'edit' )?'editsave':'addsave' ?>"/>
			<input type="hidden" name="link_id" value="<?= $link_id ?>"/>

			<table width="100%" cellspacing="0" cellpadding="2" border="0" style="margin-top: 10px">
				<tr>
					<td valign="top" style="width: 100px" align="left">
			            <p>URL</p>
					</td>
					<td valign="top" style="width: 520px" align="left">
			            <input type="text" class="text" name="new_url" style="width: 500px" value="<?= $link_url ?>"/>
					</td>
					<td valign="top" align="left">
						<input type="submit" class="button" style="width: 120px" value="Сохранить"/>
					</td>
				</tr>
				<tr>
					<td valign="top" style="width: 100px" align="left">
			            <p>Заголовок</p>
					</td>
					<td valign="top" style="width: 520px" align="left">
			            <input type="text" class="text" name="new_title" style="width: 500px" value="<?= $link_title ?>"/>
					</td>
					<td valign="top" align="left">
						&nbsp;
					</td>
				</tr>
				<tr>
					<td valign="top" style="width: 100px" align="left">
			            <p>Текст</p>
					</td>
					<td valign="top" style="width: 520px" align="left">
						<textarea name="new_text" cols="50" rows="5" class="format" style="width: 500px"><?= $link_text ?></textarea>
					</td>
					<td valign="top" align="left">
						&nbsp;
					</td>
				</tr>
			</table>
		</form>
	</body>
</html>
<?    
			exit;
	}

	$cols_per_page = 3;
	$links_sql = mysql_query("select * from links order by link_id");
?>
		<table width="100%" cellspacing="0" cellpadding="0" border="0" style="margin-top: 10px">
			<tr style="height: 28px">
				<td width="30" align="center">
					<input type="button" class="ibutton" value="" style="background-image: url(../images/admin/buttonedit.gif);" onclick="fnOpenWindow('heads.php?tab=links')" title="Редактировать метатеги"/>
				</td>
				<td width="30" align="center">
					<input type="button" class="ibutton" value="" style="background-image: url(../images/admin/buttonplus.gif)" onclick="window.location = 'index.php?tab=links&action=add'" title="Добавить ссылку"/>
				</td>
				<td colspan="<?= $cols_per_page ?>">
					<p>&nbsp;</p>
				</td>
			</tr>
<? 
	for ( $i = 0; $i < mysql_num_rows($links_sql); $i += $cols_per_page )
	{
?>
			<tr style="height: 28px">
				<td width="30">
					&nbsp;
				</td>
				<td width="30">
					&nbsp;
				</td>
<?
   		 for ( $j = 0; $j < $cols_per_page; $j++ )
		 {
			if ( $links_row = mysql_fetch_array($links_sql) )
			{
				$link_id = $links_row['link_id'];
				$link_title = $links_row['link_title'];
				$link_active = $links_row['link_active'];
?>
				<td>
					<table cellspacing="0" cellpadding="0">
						<tr>
							<td width="30" align="center">
								<input type="button" class="ibutton" value="" style="background-image: url(../images/admin/buttonminus.gif)" onclick="if (confirm('Удалить ссылку?')) window.location = 'index.php?tab=links&action=delete&link_id=<?= $link_id ?>'" title="Удалить ссылку"/>
							</td>
							<td width="30" align="center">
								<input type="button" class="ibutton" value="" style="background-image: url(../images/admin/buttonhide.gif)" onclick="window.location = 'index.php?tab=links&action=hide&link_id=<?= $link_id ?>'" title="<?= ($link_active)?"Скрыть":"Показать" ?> ссылку"/>
							</td>
							<td>
								<p><a href="index.php?tab=links&action=edit&link_id=<?= $link_id ?>"<? if (!$link_active) print " class=\"hide\"" ?>><?= $link_title ?></a></p>
							</td>
						</tr>
					</table>
				</td>
<?
			}
			else
			{
?>
				<td>
					&nbsp;
				</td>
<?
			}
		 }
?>
			</tr>
<?
	}
?>
		</table>
