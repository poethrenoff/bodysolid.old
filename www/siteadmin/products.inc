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
	$groups_sql = mysql_query("select * from groups");

	while ( $groups_row = mysql_fetch_array($groups_sql) )
	{
		$group_id = $groups_row['group_id'];
		$group_name = $groups_row['group_name'];
?>
			<tr style="height: 28px">
				<td width="30">
					&nbsp;
				</td>
				<td width="30">
					<p>&nbsp;</p>
				</td>
				<td width="30" align="center">
					<input type="button" class="ibutton" value="" style="background-image: url(../images/admin/buttonedit.gif)" onclick="fnOpenWindow('heads.php?tab=group&amp;id=<?= $group_id ?>')" title="Редактировать метатеги"/>
				</td>
				<td>
					<p><a href="index.php?tab=group&group_id=<?= $group_id ?>"><?= $group_name ?></a></p>
				</td>
			</tr>
<?    
	}
?>
		</table>
