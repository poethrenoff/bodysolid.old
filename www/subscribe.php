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
		$subject = "=?windows-1251?B?".base64_encode("Подписка на прайс-лист с сайта BodySolid.ru")."?=";

		$mes  = "E-mail                 : $subscribe_email\n";
		$mes .= "Фамилия, имя, отчество : $subscribe_name\n";
		$mes .= "Название компании      : $subscribe_company\n";
		$mes .= "Тип компании           : $subscribe_type\n";
		$mes .= "Телефон                : ($subscribe_phone_code) $subscribe_phone\n";
		$mes .= "Факс                   : ($subscribe_fax_code) $subscribe_fax\n";
		$mes .= "URL сайта              : $subscribe_url\n";
		
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
<h1>Подписка на прайс-лист</h1>
<?
	if ( $subscribe_success === 'true' )
	{
?>
<h2 style="text-align: center">Подписка оформлена успешно!</h2>
<?
	}
	else if ( $subscribe_success === 'false' )
	{
?>
<h2 style="text-align: center; color: red">При оформлении подписки произошла ошибка!</h2>
<?
	}
	
?>
<p>Если Вы хотите всегда быть в курсе наших цен и ассортимента, оформите подписку на прайс-лист. Рассылка осуществляется не чаще 1 раза в месяц.</p>		
<form id="subscribe" method="post" action="subscribe.php">
	<table cellpadding="3" cellspacing="0" style="width: 100%; margin-top: 20px; margin-left: 40px; border-collapse: collapse">
		<tr valign="top" style="height: 30px">
			<td style="width: 300px">
				<p><b>E-mail:</b> <span class="require">*</span><br/>(на этот адрес будет приходить прайс-лист)</p>
			</td>
			<td align="left">
				<input type="hidden" name="subscribe_action" value=""/>
				<input type="text" name="subscribe_email" value="" style="width: 50%" class="order"/>
			</td>
		</tr>
		<tr valign="top" style="height: 30px">
			<td>
				<p><b>Ваше имя, фамилия, отчество:</b> <span class="require">*</span></p>
			</td>
			<td align="left">
				<input type="text" name="subscribe_name" value="" style="width: 50%" class="order"/>
			</td>
		</tr>
		<tr valign="top" style="height: 30px">
			<td>
				<p><b>Название компании:</b><br/>(для юрлиц)</p>
			</td>
			<td align="left">
				<input type="text" name="subscribe_company" value="" style="width: 50%" class="order"/>
			</td>
		</tr>
		<tr valign="top" style="height: 30px">
			<td>
				<p><b>Тип компании:</b><br/>(для юрлиц)</p>
			</td>
			<td align="left">
				<select name="subscribe_type" style="width: 50%">
					<option value="" selected="selected">-- Выбор --</option>
					<option value="Оптовая">Оптовая</option>
					<option value="Розничная">Розничная</option>
				</select>
			</td>
		</tr>
		<tr valign="top" style="height: 30px">
			<td>
				<p><b>Код города, телефон:</b><br/>(для юрлиц обязательно)</p>
			</td>
			<td align="left">
				<input type="text" name="subscribe_phone_code" value="" style="width: 10%" class="order"/>
				<input type="text" name="subscribe_phone" value="" style="width: 38%" class="order"/>
			</td>
		</tr>
		<tr valign="top" style="height: 30px">
			<td>
				<p><b>Код города, факс:</b><br/>(для юрлиц обязательно)</p>
			</td>
			<td align="left">
				<input type="text" name="subscribe_fax_code" value="" style="width: 10%" class="order"/>
				<input type="text" name="subscribe_fax" value="" style="width: 38%" class="order"/>
			</td>
		</tr>
		<tr valign="top" style="height: 30px">
			<td>
				<p><b>URL сайта:</b></p>
			</td>
			<td align="left">
				<input type="text" name="subscribe_url" value="" style="width: 50%" class="order"/>
			</td>
		</tr>
		<tr valign="top" style="height: 30px">
			<td align="left">
				<input type="button" class="button" value="Подписаться" style="width: 120px"
					   onclick="if (document.forms['subscribe'].subscribe_name.value != '' &amp;&amp; 
					   	   			document.forms['subscribe'].subscribe_email.value != '') 
					   	   		{
					   	   			document.forms['subscribe'].subscribe_action.value = 'addmess';
					   	   			document.forms['subscribe'].submit();
					   	   		}
					   	   		else alert('Пожалуйста, заполните необходимые поля!');"/>
			</td>
			<td/>
		</tr>
	</table>
</form>
<?
	require 'templates/footer.inc';
?>
