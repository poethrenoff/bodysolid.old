<?
	$group_id = init_integer('group_id', 0);
?>
		<table width="100%" cellspacing="0" cellpadding="0" border="0" style="margin-top: 10px">
			<tr style="height: 28px">
				<td width="30">
					<p>&nbsp;</p>
				</td>
				<td width="30">
					<p>&nbsp;</p>
				</td>
				<td>
					<p>&nbsp;</p>
				</td>
			</tr>
<?    
	$categories_sql = mysql_query("select * from categories where group_id='$group_id'");

	while ( $categories_row = mysql_fetch_array($categories_sql) )
	{
		$category_id = $categories_row['category_id'];
		$category_name = $categories_row['category_name'];
?>
			<tr style="height: 28px">
				<td width="30">
					&nbsp;
				</td>
				<td width="30">
					<p>&nbsp;</p>
				</td>
				<td width="30" align="center">
					<input type="button" class="ibutton" value="" style="background-image: url(../images/admin/buttonedit.gif)" onclick="fnOpenWindow('heads.php?tab=category&amp;id=<?= $category_id ?>')" title="Редактировать метатеги"/>
				</td>
				<td>
					<p><?= $category_name ?></p>
				</td>
			</tr>
<?    
	}
?>
		</table>
