<?
	$article_id = init_integer('article_id', 0);

	$error = '';

	switch ( $action )
	{
		case 'delete':	// Удаление статьи
			mysql_query("delete from articles where article_id = '$article_id'");
			mysql_query("delete from heads where head_tab = 'articles' and head_id = '$article_id'");
			mysql_query("delete from article_relation where article_id = '$article_id'");
			break;

		case 'addsave':  // Сохранение добавленной статьи
		case 'editsave': // Сохранение отредактированной статьи
			$new_title = init_string('new_title', ''); $new_title = norm_string($new_title);
			$new_announce = init_string('new_announce', ''); $new_announce = norm_string($new_announce);
			$new_text = init_string('new_text',''); $new_text = norm_html($new_text); $new_search = norm_search($new_text);
			
			if ( !$new_title )
				$error = "Нельзя оставлять статью без заголовка!";
			else
			{
				if ( $action == 'editsave' )	
				{
					mysql_query("update articles set article_title = '$new_title',
												     article_announce = '$new_announce',
												     article_text = '$new_text',
												     article_search = '$new_search'
								 where article_id = '$article_id'");
				}
				else
				{
					mysql_query("insert into articles (article_title, article_announce, article_text, article_search)
								 values('$new_title', '$new_announce', '$new_text', '$new_search')");
					$article_id = mysql_insert_id();
					$action = 'edit';
				}
			}			

		case 'add':   // Добавление статьи
		case 'edit':  // Редактирование статьи
			if ( $action == 'addsave' ) $action = 'add';          	  
			if ( $action == 'editsave' ) $action = 'edit';

			if ( $action == 'edit' )
			{
				$articles_sql = mysql_query("select * from articles where article_id = '$article_id'");
				$articles_row = mysql_fetch_array($articles_sql);

				$article_title = $articles_row['article_title'];
				$article_announce = $articles_row['article_announce'];
				$article_text = htmlspecialchars( $articles_row['article_text'] );
			}
			else
			{
				$article_title = $article_announce = $article_text = '';
			}

            if ( $error ) { ?>
		<p class="baseerror"><?= $error ?></p> 
<?			} ?>
		<form id="form" name="form" method="post" onsubmit="return ( form.new_title.value.length > 0 )">
			<input type="hidden" name="tab" value="articles"/>
			<input type="hidden" name="action" value="<?= ( $action == 'edit' )?'editsave':'addsave' ?>"/>
			<input type="hidden" name="article_id" value="<?= $article_id ?>"/>

			<table width="100%" cellspacing="0" cellpadding="2" border="0" style="margin-top: 10px">
				<tr>
					<td valign="top" style="width: 100px" align="left">
			            <p>Заголовок</p>
					</td>
					<td valign="top" style="width: 520px" align="left">
			            <input type="text" class="text" name="new_title" style="width: 500px" value="<?= $article_title ?>"/>
					</td>
					<td valign="top" align="left">
						<input type="submit" class="button" style="width: 120px" value="Сохранить"/>
					</td>
				</tr>
				<tr>
					<td valign="top" style="width: 100px" align="left">
			            <p>Анонс</p>
					</td>
					<td valign="top" style="width: 520px" align="left">
						<textarea name="new_announce" cols="50" rows="5" style="width: 500px"><?= $article_announce ?></textarea>
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
						<textarea name="new_text" cols="50" rows="10" class="format" style="width: 500px"><?= $article_text ?></textarea>
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

?>
		<table width="100%" cellspacing="0" cellpadding="0" border="0" style="margin-top: 10px">
			<tr style="height: 28px">
				<td width="30" align="center">
					<input type="button" class="ibutton" value="" style="background-image: url(../images/admin/buttonedit.gif);" onclick="fnOpenWindow('heads.php?tab=articles')" title="Редактировать метатеги"/>
				</td>
				<td width="30">
					<p>&nbsp;</p>
				</td>
				<td>
					<p>&nbsp;</p>
				</td>
			</tr>
<?    
	$articles_sql = mysql_query("select * from articles order by article_id desc");

	while ( $articles_row = mysql_fetch_array($articles_sql) )
	{
		$article_id = $articles_row['article_id'];
		$article_title = $articles_row['article_title'];
?>
			<tr style="height: 28px">
				<td width="30" align="center">
					<input type="button" class="ibutton" value="" style="background-image: url(../images/admin/buttonminus.gif)" onclick="if (confirm('Удалить статью?')) window.location = 'index.php?tab=articles&action=delete&article_id=<?= $article_id ?>'" title="Удалить статью"/>
				</td>
				<td width="30" align="center">
					<input type="button" class="ibutton" value="" style="background-image: url(../images/admin/buttonedit.gif)" onclick="fnOpenWindow('heads.php?tab=articles&amp;id=<?= $article_id ?>')" title="Редактировать метатеги"/>
				</td>
				<td>
					<p><a href="index.php?tab=articles&action=edit&article_id=<?= $article_id ?>"><?= $article_title ?></a></p>
				</td>
			</tr>
<?    
	}
?>
			<tr style="height: 28px">
				<td width="30">
					&nbsp;
				</td>
				<td width="30" align="center">
					<input type="button" class="ibutton" value="" style="background-image: url(../images/admin/buttonplus.gif)" onclick="window.location = 'index.php?tab=articles&action=add'" title="Добавить статью"/>
				</td>
				<td>
					&nbsp;
				</td>
			</tr>
		</table>
