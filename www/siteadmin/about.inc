		<table width="100%" cellspacing="0" cellpadding="0" border="0" style="margin-top: 10px">
			<tr style="height: 28px">
				<td width="30" align="center">
					<input type="button" class="ibutton" value="" style="background-image: url(../images/admin/buttonedit.gif);" onclick="fnOpenWindow('heads.php?tab=about')" title="������������� ��������"/>
				</td>
				<td width="30">
					<p>&nbsp;</p>
				</td>
				<td>
					<p>&nbsp;</p>
				</td>
			</tr>
		</table>
<?
	$new_template = init_string('new_template',''); $new_template = norm_text($new_template);

	if ( $action == 'editsave' )
	{
		$ftext = fopen('../templates/about.inc','w');
		fputs($ftext,$new_template); fclose($ftext);
	}
	
	$template = htmlspecialchars(file_get_contents('../templates/about.inc'), ENT_QUOTES, 'Windows-1251');

?>
		<form id="form" name="form" method="post" onsubmit="return ( form.new_template.value.length > 0 )">
			<input type="hidden" name="tab" value="about"/>
			<input type="hidden" name="action" value="editsave"/>
			<table width="100%" cellspacing="0" cellpadding="2" border="0" style="margin-top: 10px">
				<tr>
					<td valign="top" style="width: 100px" align="left">
						<p>������ ��������</p>
					</td>
					<td valign="top" style="width: 520px" align="left">
						<textarea name="new_template" cols="50" rows="25" class="format" style="width: 500px"><?= $template ?></textarea>
					</td>
					<td valign="top" align="left">
						<input type="submit" class="button" style="width: 120px" value="���������"/>
					</td>
				</tr>
			</table>
		</form>
