<?
	require 'templates/common.inc';

	$error = array();

	if (!empty($_POST)) {
		$client_name = @iconv('UTF-8', 'Windows-1251', norm_string( init_string('client_name', '') ) );
		$client_email = @iconv('UTF-8', 'Windows-1251', norm_string( init_string('client_email', '') ) );
		$client_phone = @iconv('UTF-8', 'Windows-1251', norm_string( init_string('client_phone', '') ) );
		$client_product = @iconv('UTF-8', 'Windows-1251', norm_string( init_string('client_product', '') ) );
		$client_quantity = @iconv('UTF-8', 'Windows-1251', norm_string( init_string('client_quantity', '') ) );

		if (empty($client_name)) {
			$error['client_name'] = '�� ��������� ������������ ����';
		}
		if (empty($client_email)) {
			$error['client_email'] = '�� ��������� ������������ ����';
		}
		if (empty($client_product)) {
			$error['client_product'] = '�� ��������� ������������ ����';
		}
		if (empty($client_quantity)) {
			$error['client_quantity'] = '�� ��������� ������������ ����';
		}

		if (empty($error)) {
			$admin_email = get_preference('admin_email', 'andreisport@post.ru');
			$admin_subject = '������ �� ��������� ������';
			
			$admin_text  = "��������! � ��������-������� BodySolid ��������� ������ �� ��������� ������\n";
			$admin_text .= "\n";
			$admin_text .= "���         : $client_name\n";
			$admin_text .= "Email       : $client_email\n";
			$admin_text .= "�������     : $client_phone\n";
			$admin_text .= "�����       : $client_product\n";
			$admin_text .= "����������  : $client_quantity\n";

			@mail($admin_email, $admin_subject, $admin_text, "From: \"BodySolid.ru\" <$admin_email>\nMIME-Version: 1.0\nContent-Type: text/plain; charset=\"windows-1251\"");

			@header('Location: /discount.php?discount_complete');
			exit;
		}
	}
?>
<div class="modal-form">
    <script type="text/javascript">
        $(document).ready(function() {
            $('#discount').ajaxForm({
                target: ".simplemodal-wrap"
            });
        });
    </script>
    
    <h1 style="text-align: center; text-indent: 0px">������ �� ��������� ������</h1>

	<br/>
<?
	if (isset($_REQUEST['discount_complete'])) {
?>
	<p style="text-align: center">�������, ��� �������� �������� � ����.</p>
	<br/>
	<a style="text-align: center" href="" onclick="$.modal.close(); return false;">������� ����</a>
<?
	} else {
?>
	<form id="discount" method="post" action="/discount.php">
		<table cellpadding="3" cellspacing="0" style="width: 100%; border-collapse: collapse">
			<tr valign="top" style="height: 30px">
				<td style="width: 150px">
					<p>���� ��� <span class="require">*</span></p>
				</td>
				<td align="left">
					<input type="text" name="client_name" value="<?= @$client_name ?>" class="text" style="width: 100%"/>
<?php if (isset($error['client_name'])) { ?>
					<span class="error"><?= $error['client_name'] ?></span>
<?php } ?>
				</td>
			</tr>
			<tr valign="top" style="height: 30px">
				<td>
					<p>E-mail <span class="require">*</span></p>
				</td>
				<td align="left">
					<input type="text" name="client_email" value="<?= @$client_email ?>" class="text" style="width: 100%"/>
<?php if (isset($error['client_email'])) { ?>
					<span class="error"><?= $error['client_email'] ?></span>
<?php } ?>
				</td>
			</tr>
			<tr valign="top" style="height: 30px">
				<td>
					<p>�������</p>
				</td>
				<td align="left">
					<input type="text" name="client_phone" value="<?= @$client_phone ?>" class="text" style="width: 100%"/>
<?php if (isset($error['client_phone'])) { ?>
					<span class="error"><?= $error['client_phone'] ?></span>
<?php } ?>
				</td>
			</tr>
			<tr valign="top" style="height: 30px">
				<td>
					<p>����� <span class="require">*</span></p>
				</td>
				<td align="left">
					<input type="text" name="client_product" value="<?= @$client_product ?>" class="text" style="width: 100%"/>
<?php if (isset($error['client_product'])) { ?>
					<span class="error"><?= $error['client_product'] ?></span>
<?php } ?>
				</td>
			</tr>
			<tr valign="top" style="height: 30px">
				<td>
					<p>���������� <span class="require">*</span></p>
				</td>
				<td align="left">
					<input type="text" name="client_quantity" value="<?= @htmlspecialchars($_POST['client_quantity']) ?>" class="text" style="width: 100%"/>
<?php if (isset($error['client_quantity'])) { ?>
					<span class="error"><?= $error['client_quantity'] ?></span>
<?php } ?>
				</td>
			</tr>
			<tr valign="top" style="height: 30px">
				<td>
					&nbsp;
				</td>
				<td align="left">
					<input type="submit" value="���������" class="button"/>
				</td>
			</tr>
		</table>
	</form>
<?
	}
?>
</div>