<?
	require 'templates/common.inc';
	require 'templates/function.inc';

	// session_destroy(); session_start();

	$action = init_string('action', '');
	$product_id = init_integer('product_id', 0);
	$products = init_array('products', array());
	$basket_success = init_string('basket_success', '');
	
	$course = get_preference('course', 29.00);
	$admin_email = get_preference('admin_email', 'andreisport@post.ru');

	$discount_limit = get_preference('discount_limit', 1000.00);
	$discount_value = get_preference('discount_value', 0.9);
	$order_limit = get_preference('order_limit', 6000.00);
	
	$success_send = false;
	
	// ������� �������
	if ( !isset($_SESSION['_products']) || $action === 'clear' )
		$_SESSION['_products'] = array();

	// �������� ������, ������� ������
	if ( $action === 'send' )
	{
		$success_send = false;
			
		$client_name = norm_string( init_string('client_name', '') );
		$client_email = norm_string( init_string('client_email', '') );
		$client_phone = norm_string( init_string('client_phone', '') );
		$client_address = norm_string( init_string('client_address', '') );
		$client_comment = norm_string( init_string('client_comment', '') );		
		
		if ( $client_name && $client_email && $client_phone && $client_address && count($_SESSION['_products']) > 0 )
		{
			mysql_query("insert into clients (client_name, client_email, client_phone, client_address, client_comment)
									  values ('$client_name', '$client_email', '$client_phone', '$client_address', '$client_comment')");
			$client_id = mysql_insert_id(); $order_time = time();
			
			mysql_query("insert into orders (client_id, order_time)
									 values ('$client_id', '$order_time')");
			$order_id = mysql_insert_id();
			
			foreach ( $_SESSION['_products'] as $product_id => $product_count )
			{
				$product_id = (integer)$product_id; $product_count = (integer)$product_count;
				$products_sql = mysql_query("select * from products where product_id = '$product_id' and product_active = '1'");
				if ( $products_row = mysql_fetch_array($products_sql) && $product_count > 0 )
					mysql_query("insert into order_items (order_id, product_id, product_count)
												  values ('$order_id', '$product_id', '$product_count')");
			}

			// ���������� ��������� ��� ��������� � �������
			$order_items_sql = mysql_query("select order_items.*, products.product_name, products.product_price, products.product_no_discount, products.brand_id, categories.category_by_course
											from order_items inner join products on order_items.product_id = products.product_id inner join categories on categories.category_id = products.category_id
											where order_id = '$order_id'");
			$sum = 0; $num = 1; $basket_text = '';
			while ( $order_items_row = mysql_fetch_array($order_items_sql) ) 
			{
			  	$product_id = $order_items_row['product_id'];
			  	$product_name = $order_items_row['product_name'];
			  	$product_no_discount = $order_items_row['product_no_discount'];
				$category_by_course = $order_items_row['category_by_course'];
			  	
			  	$product_price = $order_items_row['product_price'];
			  	$product_price = round($category_by_course ? $product_price * $course : $product_price);
				if ($order_items_row['brand_id'] == 2) {
					$product_price = ceil($product_price * 0.75 / 100) * 100;
				} elseif ( !$product_no_discount && $product_price > $discount_limit ) {
					$product_price = $product_price * $discount_value;
				}
			  	
			  	$product_count = $order_items_row['product_count'];
			  	
			  	$sum += $product_price * $product_count;
			  	
			  	$basket_text .= $num++.".\t".$product_name."\t\t".$product_count." x ".$product_price." p. (".$product_count*$product_price." �.)\n";
			}
			
			$order_time = date('H:i d.m.Y', $order_time);

			// ���� � ����� ��������� ��� ���������
			$admin_subject = '����� ����� �'.$order_id;
			
			$admin_text  = "��������! � ��������-������� BodySolid �������� ����� ����� �$order_id:\n";
			$admin_text .= "\n";
			$admin_text .= "$basket_text\n";
			$admin_text .= "����� ��������� ������: $sum �.\n";
			$admin_text .= "\n";
			$admin_text .= "���         : $client_name\n";
			$admin_text .= "Email       : $client_email\n";
			$admin_text .= "��������    : $client_phone\n";
			$admin_text .= "�����       :\n";
			$admin_text .= "$client_address\n";
			$admin_text .= "����������� :\n";
			$admin_text .= "$client_comment\n";
			$admin_text .= "\n";
			$admin_text .= "������� � ������� ��������� http://www.bodysolid.ru/manager/";

			// ���� � ����� ��������� ��� �������
			$client_subject = "\"BodySolid.ru\": ��� ����� �������!";
			
			$client_text  = "��� ������������ ���������� ��������-������� BodySolid!\n";
			$client_text .= "\n";
			$client_text .= "���������� �� ��� �����. �� �������� ��������� ����������:\n";
			$client_text .= "\n";
			$client_text .= "$basket_text\n";
			$client_text .= "����� ��������� ������: $sum �.\n";
			$client_text .= "\n";
			$client_text .= "���         : $client_name\n";
			$client_text .= "Email       : $client_email\n";
			$client_text .= "��������    : $client_phone\n";
			$client_text .= "�����       :\n";
			$client_text .= "$client_address\n";
			$client_text .= "����������� :\n";
			$client_text .= "$client_comment\n";
			$client_text .= "\n";
			$client_text .= "� ��������� ����� � ���� �������� �������� ������ ��������, ����� �������� ����� �������� � ������ ������.\n";
			$client_text .= "���� �� ���������� ���������� � ����������� ���� ���������� ������, ����������, ��������� ��� �� ��������� (495) 518-14-19, 518-52-97.\n";
			$client_text .= "\n";
			$client_text .= "� ���������,\n";
			$client_text .= "��������-������� BodySolid\n";
			$client_text .= "www.bodysolid.ru\n";
			$client_text .= "(495) 518-14-19, 518-52-97.\n";
			$client_text .= "\n";
			$client_text .= "P.S. ���� ����� ��� �������� �� ���� � ��� ������ ������ �� �� ������, ����������, �������� �� ���� �� ������ andreisport@bodysolid.ru\n";
			$client_text .= "������ �������� �� ������������.\n";

			@mail($admin_email, $admin_subject, $admin_text, "From: \"BodySolid.ru\" <$admin_email>\nMIME-Version: 1.0\nContent-Type: text/plain; charset=\"windows-1251\"");
			@mail($client_email, $client_subject, $client_text, "From: \"BodySolid.ru\" <$admin_email>\nMIME-Version: 1.0\nContent-Type: text/plain; charset=\"windows-1251\"");

			$success_send = true;
		}		
		
		if ( $success_send ) session_destroy();
		
		header('Location: basket.php?basket_success='.(($success_send)?'true':'false')); exit;
	}
	
	// ���������� ������ � �������
	if ( $action === 'add' && $product_id )
	{
		if ( !isset($_SESSION['_products'][$product_id]) )
		{
			$products_sql = mysql_query("select * from products where product_id = '$product_id' and product_active = '1'");
			if ( $products_row = mysql_fetch_array($products_sql) )
				$_SESSION['_products'][$product_id] = 1;
		}			
	}

	// �������� ������ �� �������
	if ( $action === 'delete' && $product_id )
		if ( isset($_SESSION['_products'][$product_id]) )
			unset($_SESSION['_products'][$product_id]);
	
	// �������� ������� � �������
	if ( $action === 'set' )
	{
		$_SESSION['_products'] = array();
		foreach ( $products as $product_id => $product_count )
		{
			$product_id = (integer)$product_id; $product_count = (integer)$product_count;
			$products_sql = mysql_query("select * from products where product_id = '$product_id' and product_active = '1'");
			if ( $products_row = mysql_fetch_array($products_sql) && $product_count > 0 )
				$_SESSION['_products'][$product_id] = $product_count;
		}
	}

	$products = array();

	foreach ( $_SESSION['_products'] as $product_id => $product_count )
	{
		$products_sql = mysql_query("select products.*, categories.category_by_course from products
			inner join categories on categories.category_id = products.category_id where product_id = '$product_id' and product_active = '1'");
		if ( $products_row = mysql_fetch_array($products_sql) )
		{
			$product_name = $products_row['product_name'];
		  	$product_no_discount = $products_row['product_no_discount'];
			$category_by_course = $products_row['category_by_course'];

		  	$product_price = $products_row['product_price'];
			$product_price = round($category_by_course ? $product_price * $course : $product_price);
			if ($products_row['brand_id'] == 2) {
				$product_price = ceil($product_price * 0.75 / 100) * 100;
			} elseif ( !$product_no_discount && $product_price > $discount_limit ) {
				$product_price = $product_price * $discount_value;
			}

			$products[$product_id] = array( 'product_name' => $product_name,
											'product_price' => $product_price,
											'product_count' => $product_count );
		}
	}
	
	list($title, $keywords, $description) = get_head('basket', 0);

	require 'templates/header.inc';
?>
<p class="path">�������</p>
	
<h1>�������</h1>	
<?
	if ( $basket_success === 'true' )
	{
?>
<h2 style="text-align: center">�������! ��� ����� ������, �������� �������� � ���� � ��������� �����!</h2>
	
<p class="p">������ �������������� �� ������� ���� � 10 �� 19 �����. ����� �������� ������������� � ���������� �������������, ������ �� ��������� �������.</p>
<p class="p">����� ������� �� ��������� ������ ������ ����� ������ � ������� ����� �� ��������� <b>(495) 518-14-19, 518-52-97</b>.</p>
<?
	}
	else if ( $basket_success === 'false' )
	{
?>
<h2 style="text-align: center; color: #ff0000">������ ��� �������� ������!</h2>
<?
	}	
	
	if ( $action === 'order' )
	{
		$client_data = array( 'client_name' => '', 'client_email' => '', 'client_phone' => '', 'client_address' => '', 'client_comment' => '' );
		if ( isset($_SESSION['_client_name']) ) $client_data['client_name'] = $_SESSION['_client_name'];
		if ( isset($_SESSION['_client_email']) ) $client_data['client_email'] = $_SESSION['_client_email'];
		if ( isset($_SESSION['_client_phone']) ) $client_data['client_phone'] = $_SESSION['_client_phone'];
		if ( isset($_SESSION['_client_address']) ) $client_data['client_address'] = $_SESSION['_client_address'];
		if ( isset($_SESSION['_client_comment']) ) $client_data['client_comment'] = $_SESSION['_client_comment'];

		PrintBasket($products, false);
		PrintOrderForm($client_data, true);
	}
	else if ( $action === 'preview' )
	{
		$client_data = array( 'client_name' => '', 'client_email' => '', 'client_phone' => '', 'client_address' => '', 'client_comment' => '' );
		$client_data['client_name'] = $_SESSION['_client_name'] = htmlspecialchars( norm_text( init_string('client_name', '') ), ENT_QUOTES, 'Windows-1251' );
		$client_data['client_email'] = $_SESSION['_client_email'] = htmlspecialchars( norm_text( init_string('client_email', '') ), ENT_QUOTES, 'Windows-1251' );
		$client_data['client_phone'] = $_SESSION['_client_phone'] = htmlspecialchars( norm_text( init_string('client_phone', '') ), ENT_QUOTES, 'Windows-1251' );
		$client_data['client_address'] = $_SESSION['_client_address'] = htmlspecialchars( norm_text( init_string('client_address', '') ), ENT_QUOTES, 'Windows-1251' );
		$client_data['client_comment'] = $_SESSION['_client_comment'] = htmlspecialchars( norm_text( init_string('client_comment', '') ), ENT_QUOTES, 'Windows-1251' );

		PrintBasket($products, false);
		PrintOrderForm($client_data, false);
	}
	else if ( count($products) > 0 )
	{
		PrintBasket($products, true);
	}
	else if ( !$basket_success )
	{
?>
<h2 style="text-align: center">���� ������� �����!</h2>
<?
	}

	require 'templates/footer.inc';
?>