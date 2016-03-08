<?
	require 'templates/common.inc';
	require 'templates/function.inc';

	$action = init_string('action', '');
	
	$search_rows = array();

	// Контекстный поиск
	$search_text = init_string('search_text', '');

	$restore_search_text = htmlspecialchars( norm_text($search_text), ENT_QUOTES, 'Windows-1251' );
	$search_text = norm_html( $search_text );
	
	// Поиск по каталогу
	$query_string_name = getSearchString( $search_text, 'product_name' );
	$query_string_index = getSearchString( $search_text, 'product_search' );
	
	if ( $query_string_name && $query_string_index )
	{
		$query_string = '( ' . $query_string_name . ' or ' . $query_string_index . ' )';
		
		$search_sql = mysql_query("select * from products where $query_string and product_active = '1' order by product_name");
		
		if ( mysql_num_rows($search_sql) )
			while ( $row = mysql_fetch_array($search_sql) )
				$search_rows[] = array( 'link' => 'products.php?tab=product&amp;product_id='.$row['product_id'], 
										'name' => $row['product_name'], 'page' => 'Каталог' );
	}
	
	// Поиск по статьям
	$query_string = getSearchString( $search_text, 'article_search' );
	if ( $query_string )
	{
		$search_sql = mysql_query("select * from articles where $query_string");

		if ( mysql_num_rows($search_sql) )
			while ( $row = mysql_fetch_array($search_sql) )
				$search_rows[] = array( 'link' => 'articles.php?article_id='.$row['article_id'], 
										'name' => $row['article_title'], 'page' => 'Статьи' );
	}

	list($title, $keywords, $description) = get_head('search', 0);
	$title = "БодиСолид - Результаты поиска: $restore_search_text (".count($search_rows).")";

	require 'templates/header.inc';	
?>
<p class="path">Поиск</p>
	
<h1>Результаты поиска</h1>
<?
	$search_cnt = count($search_rows);
	
	if ( !$search_cnt )
	{
?>			
<h2 style="text-align: center">Ничего не найдено!</h2>
<?	
	}
	else
	{	
?>
<table cellpadding="3" cellspacing="0" style="width: 100%">
<?
		for ( $i = 0; $i < $search_cnt; $i++ )
		{
			$search_link = $search_rows[$i]['link'];
			$search_name = $search_rows[$i]['name'];
			$search_page = $search_rows[$i]['page'];
?>
	<tr>
		<td style="width: 20px; text-align: right">
			<?= $i+1 ?>.
		</td>
		<td>
			<a href="<?= $search_link ?>"><?= $search_name ?></a> (<?= $search_page ?>)
		</td>
	</tr>
<?
		}
?>
</table>
<?
	}

	require 'templates/footer.inc';
?>