<?
	require 'templates/common.inc';

	$article_id = init_integer('article_id', 0);

	list($title, $keywords, $description) = get_head('articles', $article_id);

	$subscribe_action = init_string('subscribe_action', '');

	$subscribe_name = init_string('subscribe_name', '');
	$subscribe_email = init_string('subscribe_email', '');
	$subscribe_company = init_string('subscribe_company', '');
	$subscribe_type = init_string('subscribe_type', '');
	$subscribe_phone_code = init_string('subscribe_phone_code', '');
	$subscribe_phone = init_string('subscribe_phone', '');		
	$subscribe_fax_code = init_string('subscribe_fax_code', '');
	$subscribe_fax = init_string('subscribe_fax', '');
	$subscribe_url = init_string('subscribe_url', '');
	
	$subscribe_success = init_string('subscribe_success', '');

	if ( $subscribe_action == 'addmess' && $subscribe_name && $subscribe_email && $_SERVER['REQUEST_METHOD'] == 'POST' && 
		 ereg($_SERVER['HTTP_HOST'].$_SERVER['PHP_SELF'], $_SERVER['HTTP_REFERER']) )
	{
		$subject = "=?windows-1251?B?".base64_encode("�������� �� �����-���� � ����� BodySolid.ru")."?=";

		$mes  = "E-mail                 : $subscribe_email\n";
		$mes .= "�������, ���, �������� : $subscribe_name\n";
		$mes .= "�������� ��������      : $subscribe_company\n";
		$mes .= "��� ��������           : $subscribe_type\n";
		$mes .= "�������                : ($subscribe_phone_code) $subscribe_phone\n";
		$mes .= "����                   : ($subscribe_fax_code) $subscribe_fax\n";
		$mes .= "URL �����              : $subscribe_url\n";
		
		$adminmail = get_preference( 'subscribe_email', 'andreisport@post.ru' );
		
		if ( @mail($adminmail, $subject, $mes, "From: \"BodySolid.ru\" <admin@bodysolid.ru>\nMIME-Version: 1.0\nContent-Type: text/plain; charset=\"windows-1251\"") )
		{
			header('Location: subscribe.php?subscribe_success=true'); exit;
		}
		else
		{
			header('Location: subscribe.php?subscribe_success=false'); exit;
		}
	}
	
	require 'templates/header.inc';
?>
<h1>�������� �� �����-����</h1>
<?
	if ( $subscribe_success === 'true' )
	{
?>
<h2 style="text-align: center">�������� ��������� �������!</h2>
<?
	}
	else if ( $subscribe_success === 'false' )
	{
?>
<h2 style="text-align: center; color: red">��� ���������� �������� ��������� ������!</h2>
<?
	}
	
?>
<p>���� �� ������ ������ ���� � ����� ����� ��� � ������������, �������� �������� �� �����-����. �������� �������������� �� ���� 1 ���� � �����.</p>		
<form id="subscribe" method="post" action="subscribe.php">
	<table cellpadding="3" cellspacing="0" style="width: 100%; margin-top: 20px; margin-left: 40px; border-collapse: collapse">
		<tr valign="top" style="height: 30px">
			<td style="width: 300px">
				<p><b>E-mail:</b> <span class="require">*</span><br/>(�� ���� ����� ����� ��������� �����-����)</p>
			</td>
			<td align="left">
				<input type="hidden" name="subscribe_action" value=""/>
				<input type="text" name="subscribe_email" value="" style="width: 50%" class="order"/>
			</td>
		</tr>
		<tr valign="top" style="height: 30px">
			<td>
				<p><b>���� ���, �������, ��������:</b> <span class="require">*</span></p>
			</td>
			<td align="left">
				<input type="text" name="subscribe_name" value="" style="width: 50%" class="order"/>
			</td>
		</tr>
		<tr valign="top" style="height: 30px">
			<td>
				<p><b>�������� ��������:</b><br/>(��� �����)</p>
			</td>
			<td align="left">
				<input type="text" name="subscribe_company" value="" style="width: 50%" class="order"/>
			</td>
		</tr>
		<tr valign="top" style="height: 30px">
			<td>
				<p><b>��� ��������:</b><br/>(��� �����)</p>
			</td>
			<td align="left">
				<select name="subscribe_type" style="width: 50%">
					<option value="" selected="selected">-- ����� --</option>
					<option value="�������">�������</option>
					<option value="���������">���������</option>
				</select>
			</td>
		</tr>
		<tr valign="top" style="height: 30px">
			<td>
				<p><b>��� ������, �������:</b><br/>(��� ����� �����������)</p>
			</td>
			<td align="left">
				<input type="text" name="subscribe_phone_code" value="" style="width: 10%" class="order"/>
				<input type="text" name="subscribe_phone" value="" style="width: 38%" class="order"/>
			</td>
		</tr>
		<tr valign="top" style="height: 30px">
			<td>
				<p><b>��� ������, ����:</b><br/>(��� ����� �����������)</p>
			</td>
			<td align="left">
				<input type="text" name="subscribe_fax_code" value="" style="width: 10%" class="order"/>
				<input type="text" name="subscribe_fax" value="" style="width: 38%" class="order"/>
			</td>
		</tr>
		<tr valign="top" style="height: 30px">
			<td>
				<p><b>URL �����:</b></p>
			</td>
			<td align="left">
				<input type="text" name="subscribe_url" value="" style="width: 50%" class="order"/>
			</td>
		</tr>
		<tr valign="top" style="height: 30px">
			<td align="left">
				<input type="button" class="button" value="�����������" style="width: 120px"
					   onclick="if (document.forms['subscribe'].subscribe_name.value != '' &amp;&amp; 
					   	   			document.forms['subscribe'].subscribe_email.value != '') 
					   	   		{
					   	   			document.forms['subscribe'].subscribe_action.value = 'addmess';
					   	   			document.forms['subscribe'].submit();
					   	   		}
					   	   		else alert('����������, ��������� ����������� ����!');"/>
			</td>
			<td/>
		</tr>
	</table>
</form>
<?
	require 'templates/footer.inc';
?>
