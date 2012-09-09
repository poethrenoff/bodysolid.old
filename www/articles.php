<?
	require 'templates/common.inc';

	$article_id = init_integer('article_id', 0);

	list($title, $keywords, $description) = get_head('articles', $article_id);

	require 'templates/header.inc';

	$articles_sql = mysql_query("select * from articles where article_id = '$article_id'");

	if ( $articles_row = mysql_fetch_array($articles_sql) )
	{
		$article_id = $articles_row['article_id'];
		$article_title = $articles_row['article_title'];
		$article_text = $articles_row['article_text'];
?>
<p class="path"><a class="path" href="articles.php">Статьи</a> - <?= $article_title ?></p>
	
<h1><?= $article_title ?></h1><?= $article_text ?> 
<?	
	}
	else
	{
?>
<p class="path">Статьи</p>
	
<h1>Статьи</h2>
<?
	    $articles_sql = mysql_query("select * from articles order by article_id desc");

		while ( $articles_row = mysql_fetch_array($articles_sql) )
		{
			$article_id = $articles_row['article_id'];
			$article_title = $articles_row['article_title'];
			$article_announce = $articles_row['article_announce'];
?>
<p class="p"><b><a href="?article_id=<?= $article_id ?>"><?= $article_title ?></a></b></p><p class="p" style="margin-bottom: 20px"><?= $article_announce ?></p>
<?
		}
	}

	require 'templates/footer.inc';
?>