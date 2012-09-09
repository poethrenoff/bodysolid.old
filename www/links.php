<?
	require 'templates/common.inc';

	list($title, $keywords, $description) = get_head('links', 0);

	require 'templates/header.inc';
?>
<p class="path">Ссылки</p>

<h1>Ссылки</h1>
<?
	$max_link_list = 20;
	$page = init_integer('page',0);

	$links_cnt_sql = mysql_query("select count(*) from links where link_active = '1'");

	list($links_cnt) = mysql_fetch_row($links_cnt_sql);

	if ( $page * $max_link_list >= $links_cnt )
		$page = floor( ($links_cnt - 1) / $max_link_list );
	if ( $page < 0 ) $page = 0;

	$link_str = '';
	if ( $links_cnt >= $max_link_list )
	{
		for ( $p = 0; $p < $links_cnt / $max_link_list ; $p++ )
		{
			if ( $p == $page )
				$link_str .= "<b>".($p + 1)."</b>&nbsp;&nbsp;";
			else
				$link_str .= "<a href=\"links.php?page=$p\">".($p + 1)."</a>&nbsp;&nbsp;";
		}          
		$link_str = "Страницы:&nbsp;&nbsp;".substr($link_str, 0, strlen($link_str) - 12);
	}    

	$link_start_list = $page * $max_link_list;
	$links_sql = mysql_query("select * from links where link_active = '1'
								 limit $link_start_list, $max_link_list");

	while ( $links_row = mysql_fetch_array($links_sql) )
	{
		$link_id = $links_row['link_id'];
		$link_url = $links_row['link_url'];
		$link_title = $links_row['link_title'];
		$link_text = $links_row['link_text'];
?>
<p><a href="<?= $link_url ?>"><b><?= $link_title ?></b></a></p><div style="margin: 0px 0px 10px 50px; text-align: left"><?= $link_text ?></div>
<?
	}
	
	if ( $links_cnt > $max_link_list )
	{	
?>
<p class="r" style="margin-top: 20px"><?= $link_str ?></p>
<?
	}	

	require 'templates/footer.inc';
?>