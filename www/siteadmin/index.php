<?
	require "../templates/common.inc";

	$tabs = array('index' => 'Главная', 
				  'products' => 'Тренажеры',
				  		'group' => 'Тренажеры',
				  'about' => 'Компания',
				  'delivery' => 'Доставка',
				  'articles' => 'Статьи',
				  'links' => 'Ссылки');

	$tab = init_string('tab', 'index');
	if ( !@$tabs[$tab] ) $tab = 'index';	
	
	$action = init_string('action', '');	
?>
<html>
	<head>
		<title>"БодиСолид" :: Администрирование сайта</title>
		<link rel="stylesheet" href="../styles/admin.css" type="text/css"/>
		<script type="text/javascript">
			function fnOpenWindow(sUrl)
			{
				var iWidth = 600, iHeight = 265, iPosX = screen.width/2 - iWidth/2, iPosY = screen.height/2 - iHeight/2;
				oWin = window.open(sUrl,'oWindow','left=' + iPosX + ', top=' + iPosY + ',width=' + iWidth + ', height=' + iHeight + ', toolbar=0, status=0, menubar=0, resizable=1, scrollbars=1');
				oWin.focus();
			}
		</script>
	</head>
	<body>
<?
	fnPrintHeader( $tabs, $tab );

	if ( file_exists( $tab.'.inc' ) )
		require $tab.'.inc';
?>
	</body>
</html>
<?
	function fnPrintHeader( $tabs, $tab )
	{
		if ( $tab == 'group' ) $tab = 'products';
?>
		<table width="100%" cellspacing="0" cellpadding="3" style="border: 1px solid #92BA85; margin-bottom: 10px">
			<tr valign="top">
<?
		foreach ( $tabs as $tab_key => $tab_value )
		{
			if ( $tab_key == 'group' ) continue;
			
			if ( $tab_key == $tab )
			{
?>
				<td width="<?= round( 55/( count($tabs) ) ) ?>%" bgcolor="#F1F9F0" style="border-right: 1px solid #92BA85">
					<p class="c"><nobr><a href="<?= $_SERVER['PHP_SELF'].'?tab='.$tab_key ?>"><?= $tab_value ?></nobr></a></p>
				</td>
<?
			}
			else
			{
?>
				<td width="<?= round( 55/( count($tabs) ) ) ?>%" bgcolor="#E3F3E1" style="border-right: 1px solid #92BA85; border-bottom: 1px solid #92BA85">
					<p class="c"><nobr><a href="<?= $_SERVER['PHP_SELF'].'?tab='.$tab_key ?>"><?= $tab_value ?></a></nobr></p>
				</td>
<?
			}
		}
?>
				<td width="45%" valign="top" bgcolor="#E3F3E1" style="border-bottom: 1px solid #92BA85">
					<p class="h">Интернет-магазин "БодиСолид" (администрирование сайта)</p>
				</td>
			</tr>
			<tr>
				<td bgcolor="#F1F9F0" align="right" colspan="<?= count($tabs) + 1 ?>">
					<p>&nbsp;</p>
				</td>
			</tr>
		</table>
<?    		  
	}
?>