<?
	require "../templates/common.inc";

	$order_id = init_integer('order_id', 0);
	$action = init_string('action', '');
	
	$course = get_preference('course', 28.00);
	$discount_limit = get_preference('discount_limit', 1000.00);
	$discount_value = get_preference('discount_value', 0.9);
	
	if ( $action === 'delete' )
	{
		list( $client_id ) = mysql_fetch_row( mysql_query("select client_id from orders where order_id = '$order_id'") );
		mysql_query("delete from clients where client_id = '$client_id'");
		mysql_query("delete from order_items where order_id = '$order_id'");
		mysql_query("delete from orders where order_id = '$order_id'");
	}

	if ( $action === 'show' )
		mysql_query("update orders set order_active = '1' where order_id = '$order_id'");
	if ( $action === 'hide' )
		mysql_query("update orders set order_active = '0' where order_id = '$order_id'");
	
	if ( $action )
	{
		header('Location: '.$_SERVER['PHP_SELF']); exit;
	}
?>
<html>
	<head>
		<title>"БодиСолид" :: Консоль менеджера</title>
		<meta http-equiv="Content-Type" content="text/html; charset=windows-1251"/>
		<meta http-equiv="refresh" content="60; url=index.php"/>
		<link rel="stylesheet" href="../styles/admin.css" type="text/css"/> 
	</head>
	<body>
		<table cellpadding="5" cellspacing="0" style="width: 100%">
<?
	$orders_sql = mysql_query("select orders.*, clients.*
							   from orders inner join clients on orders.client_id = clients.client_id
							   order by orders.order_time desc");

	while ( $orders_row = mysql_fetch_array($orders_sql) ) 
	{
	  	$order_id = $orders_row['order_id'];
	  	$order_active = $orders_row['order_active'];
	  	$order_time = date('H:i d.m.Y', $orders_row['order_time']);
	  	
	  	if ( !$order_active ) 
	  	{
?>
			<tr valign="top">
				<td colspan="2" style="border-bottom: 1px solid #9285BA">
					<p><b><?= $order_time ?></b> (<?= $order_id ?>)</p>
				</td>
				<td style="width: 10%; border-bottom: 1px solid #9285BA">
					<form id="form<?= $order_id ?>" action="<?= $_SERVER['PHP_SELF'] ?>" method="post">
						<input type="hidden" name="action" value="show"/>
						<input type="hidden" name="order_id" value="<?= $order_id ?>"/>
						<table cellpadding="1" cellspacing="0" style="width: 100%">
							<tr valign="top">
								<td align="center">
									<input type="submit" class="button" style="width: 120px; margin: 0px" value="Показать"/>
								</td>
							</tr>
						</table>
					</form>
				</td>
			</tr>    
<?	  		
	  	}
	  	else
	  	{
		  	$client_name = $orders_row['client_name'];
		  	$client_email = $orders_row['client_email'];
		  	$client_phone = $orders_row['client_phone'];
		  	$client_address = $orders_row['client_address'];
		  	$client_comment = $orders_row['client_comment'];
?>
			<tr valign="top">
				<td colspan="3">
					<p><b><?= $order_time ?></b> (<?= $order_id ?>)</p>
				</td>
			</tr>    
			<tr valign="top">
				<td style="width: 50%">
					<table cellpadding="1" cellspacing="0" style="width: 100%">
						<tr valign="top">
							<td style="width: 20%">
								<p><b>ФИО:</b></p>
							</td>
							<td style="width: 80%">
								<p><?= $client_name ?></p>
							</td>
						</tr>
						<tr valign="top">
							<td>
								<p><b>Email:</b></p>
							</td>
							<td>
								<p><a href="mailto:<?= $client_email ?>"><?= $client_email ?></a></p>
							</td>
						</tr>
						<tr valign="top">
							<td>
								<p><b>Телефоны:</b></p>
							</td>
							<td>
								<p><?= $client_phone ?></p>
							</td>
						</tr>
					</table>
				</td>
				<td style="width: 40%">
					<table cellpadding="1" cellspacing="0" style="width: 100%; border-collapse: collapse">
<?
			$order_items_sql = mysql_query("select order_items.*, products.product_name, products.product_price, products.product_no_discount
											from order_items inner join products on order_items.product_id = products.product_id
											where order_id = '$order_id'");
			
			$sum = 0; $num = 1;
			while ( $order_items_row = mysql_fetch_array($order_items_sql) ) 
			{
			  	$product_id = $order_items_row['product_id'];
			  	$product_name = $order_items_row['product_name'];
			  	$product_no_discount = $order_items_row['product_no_discount'];
			  	
			  	$product_price = $order_items_row['product_price'];
				if ( !$product_no_discount && $product_price > $discount_limit )
					$product_price *= $discount_value;
			  	$product_price = round($product_price * $course);

			  	$product_count = $order_items_row['product_count'];
			  	
			  	$sum += $product_price * $product_count;
?>
						<tr valign="top" style="background-color: <?= !($num % 2)?"#FFFFFF":"#F8FCF8" ?>">
							<td style="width: 5%; border: 1px solid #92BA85">
								<p class="r"><?= $num++.'.' ?></p>
							</td>
							<td style="width: 60%; border: 1px solid #92BA85">
								<p><?= $product_name ?></p>
							</td>
							<td style="width: 5%; border: 1px solid #92BA85">
								<p class="c"><?= $product_count ?></p>
							</td>
							<td style="width: 15%; border: 1px solid #92BA85">
								<p class="r"><?= $product_price ?></p>
							</td>
							<td style="width: 15%; border: 1px solid #92BA85">
								<p class="r"><?= $product_count*$product_price ?></p>
							</td>
						</tr>
<?
			}
?>
						<tr valign="top">
							<td colspan="4">
								<p class="r"><b>Итого:</b></p>
							</td>
							<td>
								<p class="r"><?= $sum ?></p>
							</td>
						</tr>
					</table>
				</td>
				<td style="width: 10%">
					<form id="form<?= $order_id ?>" action="<?= $_SERVER['PHP_SELF'] ?>" method="post">
						<input type="hidden" name="action" value=""/>
						<input type="hidden" name="order_id" value="<?= $order_id ?>"/>
						<table cellpadding="1" cellspacing="0" style="width: 100%">
							<tr valign="top">
								<td align="center">
									<input type="button" class="button" style="width: 120px; margin: 0px" value="Скрыть"
										onclick="document.forms['form<?= $order_id ?>'].action.value = 'hide'; document.forms['form<?= $order_id ?>'].submit()" />
								</td>
							</tr>
							<tr valign="top">
								<td align="center">
									<input type="button" class="button" style="width: 120px; margin: 0px" value="Удалить"
										onclick="if ( confirm('Вы действительно хотите удалить заказ?') ) { document.forms['form<?= $order_id ?>'].action.value = 'delete'; document.forms['form<?= $order_id ?>'].submit() }" />
								</td>
							</tr>
						</table>
					</form>
				</td>
			</tr>
			<tr valign="top">
				<td style="border-bottom: 1px solid #92BA85">
					<table cellpadding="1" cellspacing="0" style="width: 100%">
						<tr valign="top">
							<td style="width: 20%">
								<p><b>Адрес:</b></p>
							</td>
							<td style="width: 80%">
								<div style="width: 100%; background-color: #F8FCF8">
									<p><?= $client_address ?></p>
								</div>
							</td>
						</tr>
					</table>
				</td>
				<td colspan="2" style="border-bottom: 1px solid #92BA85">
					<table cellpadding="1" cellspacing="0" style="width: 100%">
						<tr valign="top">
							<td style="width: 20%">
								<p><b>Комментарий:</b></p>
							</td>
							<td style="width: 80%">
								<div style="width: 100%; background-color: #F8FCF8">
									<p><?= $client_comment ?></p>
								</div>
							</td>
						</tr>
					</table>
				</td>
			</tr>
<?
		}
	}
?>
		</table>
	</body>
</html>
