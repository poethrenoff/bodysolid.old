<?	
	$group_id = init_integer('group_id', 0);
	
	$groups_sql = mysql_query("select * from groups where group_id = '$group_id'");

	if ( $groups_row = mysql_fetch_array($groups_sql) )
	{
		$group_name = $groups_row['group_name'];
		$group_comment = $groups_row['group_comment'];

		$group_comment = '<p class="p">'.join('</p><p class="p">', explode( "\r\n\r\n", trim($group_comment) ) ).'</p>';
?>
<p class="path"><?= $group_name ?></p>

<h1><?= $group_name ?></h1>

<?= $group_comment ?> 
	
<div class="indent"></div>

<?
		$max_category_col = 3;
		
		$categories_sql = mysql_query("select * from categories 
								       where group_id = '$group_id' and category_active = '1' 
									   order by category_order");
		$categories_cnt = mysql_num_rows($categories_sql);	

		if ( $categories_cnt )
		{
			$categories_rows = array();		
			while ( $categories_rows[] = mysql_fetch_array($categories_sql) );	
?>

<table cellpadding="2" cellspacing="0" style="width: 100%">
<?	
		    for ( $i = 0; $i < $categories_cnt; $i += $max_category_col )
			{
?>
	<tr align="center" valign="top">
<?
				for ( $j = 0; $j < $max_category_col; $j++ )
				{
?>
		<td style="width: <?= round(100/$max_category_col) ?>%">
<?
					if ( $i + $j < $categories_cnt )
					{
						$category_id = $categories_rows[$i + $j]['category_id'];
						$category_name = $categories_rows[$i + $j]['category_name'];
?>
			<a href="?tab=category&amp;category_id=<?= $category_id ?>"><?= $category_name ?></a>
<?
					}
					else
					{
?>
			<p>&nbsp;</p>
<?
	   				}
?>
		</td>
<?
				}
?>
	</tr>
	<tr align="center" valign="bottom">
<?
				for ( $j = 0; $j < $max_category_col; $j++ )
				{
?>
		<td style="width: <?= round(100/$max_category_col) ?>%; padding-bottom: 20px">
<?
					if ( $i + $j < $categories_cnt )
					{
						$category_id = $categories_rows[$i + $j]['category_id'];
						$category_name = $categories_rows[$i + $j]['category_name'];
						$category_picture = $categories_rows[$i + $j]['category_picture'];
						
						if ( !file_exists($_SERVER['DOCUMENT_ROOT'].$category_images.$category_picture) ||
							 !is_file($_SERVER['DOCUMENT_ROOT'].$category_images.$category_picture) )
							$category_picture = $category_picture_default;
					
?>
			<a href="?tab=category&amp;category_id=<?= $category_id ?>"><img src="<?= $category_images.$category_picture ?>" class="product" alt="<?= $category_name ?>" title="<?= $category_name ?>"/></a>
<?
					}
					else
					{
?>
			<p>&nbsp;</p>
<?
	   				}
?>
		</td>
<?
				}
?>
	</tr>
<?
			}
?>
</table>
<?	
		}
	}
?>