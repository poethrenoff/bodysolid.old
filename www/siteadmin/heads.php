<?
	require "../templates/common.inc";

	$tabs = array('index' => 'Главная', 
				  'products' => 'Тренажеры',
					  'group' => 'Тренажеры (группы)',
					  'category' => 'Тренажеры (категории)',
				  'about' => 'О компании',
				  'delivery' => 'Доставка',
				  'articles' => 'Статьи',
				  'links' => 'Ссылки',
				  'service' => 'Услуги' );

	$tab = init_string('tab', 'index');
	if ( !@$tabs[$tab] ) $tab = 'index';	

	$id = init_string('id', 0);

	$action = init_string('action', '');

	if ( $action == 'savemeta' )
	{
		$new_title = norm_string(init_string('new_title',''));
		$new_keywords = norm_string(init_string('new_keywords',''));
		$new_description = norm_string(init_string('new_description',''));
      
		list($head_cnt) = mysql_fetch_row(mysql_query("select count(*) from heads where head_tab = '$tab' and head_id='$id'"));
		
		if ( $head_cnt )
		{
			mysql_query("update heads set head_title = '$new_title',
										  head_keywords = '$new_keywords',
										  head_description = '$new_description'
						 where head_tab = '$tab' and head_id='$id'");
		}
		else
		{
			mysql_query("insert into heads (head_tab, head_id, head_title, head_keywords, head_description)
									 values('$tab', '$id', '$new_title', '$new_keywords', '$new_description')");
		}
	}

?>
<html>
	<head>
		<title>"БодиСолид" :: <?= $tabs[$tab] ?> :: Редактирование метатегов</title>
		<link rel="stylesheet" href="../styles/admin.css" type="text/css"/>
		<script type="text/javascript">
			if ( !window.opener ) window.location = 'index.php';
		</script>
	</head>
	<body>
<?
	list($title, $keywords, $description) = get_head($tab, $id);
?>
		<form action="<?= $_SERVER['PHP_SELF'] ?>?tab=<?= $tab ?>&amp;id=<?= $id ?>" method="post">
			<input type="hidden" name="action" value="savemeta"/>
			<table width="100%" cellspacing="0" cellpadding="2">
				<tr valign="top">
					<td width="15%">
						<p>Заголовок</p>
					</td>
					<td width="85%">
						<input type="text" class="text" name="new_title" style="width: 100%" value="<?= $title ?>"/>
					</td>
				</tr>
				<tr valign="top">
					<td>
						<p>Ключевые слова</p>
					</td>
					<td>
						<textarea name="new_keywords" cols="50" rows="5" style="width: 100%"><?= $keywords  ?></textarea>
					</td>
				</tr>
				<tr valign="top">
					<td>
						<p>Описание</p>
					</td>
					<td>
						<textarea name="new_description" cols="50" rows="5" style="width: 100%"><?= $description  ?></textarea>
					</td>
				</tr>
				<tr valign="top">
					<td>
						<p>&nbsp;</p>
					</td>
					<td>
						<input type="submit" class="button" style="width: 120px" value="Сохранить"/>
						&nbsp;&nbsp;&nbsp;
						<input type="button" class="button" style="width: 120px" value="Закрыть" onclick="window.close()"/>
					</td>
				</tr>
			</table>			
		</form>
	</body>
</html>