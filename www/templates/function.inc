<?
	function PrintProducts( $products_rows, $categories_row, $show_description = true, $show_discount = false )
	{
		$max_product_col = 3; $products_cnt = count($products_rows);
		
		global $product_preview_images, $product_preview_picture_default, $course, $discount_limit, $discount_value, $image_base;
		
		$category_by_course = $categories_row['category_by_course'];
?>

<table cellpadding="3" cellspacing="0" style="width: 100%">
<?
	    for ( $i = 0; $i < $products_cnt; $i += $max_product_col )
		{
?>
	<tr align="center" valign="top">
<?
			for ( $j = 0; $j < $max_product_col; $j++ )
			{
?>
		<td style="width: <?= round(100/$max_product_col) ?>%; text-align: center; padding-top: 10px;">
<?
				if ( $i + $j < $products_cnt )
				{
					$product_id = $products_rows[$i + $j]['product_id'];
					$product_name = $products_rows[$i + $j]['product_name'];
?>
			<a href="products.php?tab=product&amp;product_id=<?= $product_id ?>"><?= $product_name ?></a>
<?
				}
				else
				{
?>
			&nbsp;
<?
   				}
?>
		</td>
<?
			}
?>
	</tr>
	<tr align="center" valign="top">
<?
			for ( $j = 0; $j < $max_product_col; $j++ )
			{
?>
		<td style="height: 150px">
<?
				if ( $i + $j < $products_cnt )
				{
					$product_id = $products_rows[$i + $j]['product_id'];
					$product_name = $products_rows[$i + $j]['product_name'];
					$product_picture = $products_rows[$i + $j]['product_picture'];
					
					if ( !file_exists($_SERVER['DOCUMENT_ROOT'].$product_preview_images.$product_picture) ||
						 !is_file($_SERVER['DOCUMENT_ROOT'].$product_preview_images.$product_picture) )
						$product_picture = $product_preview_picture_default;
					
					if ($show_discount) {
?>
			<a href="discount.php" onclick="return discount('<?= $product_name ?>')" class="discount"><img src="/images/discount.png"></a>
<?
					}
?>
			<a href="products.php?tab=product&amp;product_id=<?= $product_id ?>"><img src="<?= $image_base.$product_preview_images.$product_picture ?>" class="product" alt="<?= $product_name ?>" title="<?= $product_name ?>"/></a>
<?
				}
				else
				{
?>
			&nbsp;
<?
   				}
?>
		</td>
<?
			}
?>
	</tr>
<?
			if ($show_description) {
?>
	<tr align="center" valign="top">
<?
				for ( $j = 0; $j < $max_product_col; $j++ )
				{
?>
		<td>
<?
					if ( $i + $j < $products_cnt )
					{
						$product_id = $products_rows[$i + $j]['product_id'];
						$product_price = $products_rows[$i + $j]['product_price'];
						$product_discount = $products_rows[$i + $j]['product_discount'];
						$product_short_desc = $products_rows[$i + $j]['product_short_desc'];
						$product_no_discount = $products_rows[$i + $j]['product_no_discount'];
						
						$product_price = $product_price_old = round($category_by_course ? $product_price * $course : $product_price);
						if ($products_rows[$i + $j]['brand_id'] == 2) {
							$product_price = ceil($product_price * 0.8 / 100) * 100;
						} elseif ( $product_discount ) {
							$product_price = ceil($product_price * (100 - $product_discount) / 100 / 100) * 100;
						} elseif ( !$product_no_discount && $product_price > $discount_limit ) {
							$product_price = $product_price * $discount_value;
						}
?>
			<table cellpadding="2" cellspacing="0" style="width: 100%">
				<tr>
					<td style="width: 50%; text-align: center">
<?						if ($product_price < $product_price_old) { ?>
						<s style="color: #666666"><?= $product_price_old ?> �.</s><br>
<? 						} ?>
						<b><?= $product_price ?> �.</b>
					</td>
					<td style="width: 50%" align="right">
						<a href="basket.php?action=add&amp;product_id=<?= $product_id ?>" style="margin-right: 35px"><img src="images/basket.gif" alt="������" title="������"/></a>
					</td>
				</tr>
				<tr>
					<td colspan="2" style="text-align: center">
						<?= $product_short_desc ?>
					</td>
				</tr>
			</table>
	
<?
					}
					else
					{
?>
			&nbsp;
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
		}
?>
</table>
<?	
	}

	function PrintBasket( $products, $edit = false )
	{
		global $action, $order_limit;
?>
<p class="p" style="margin-bottom: 20px">������ ��������� ���� ���������� �������:</p> 
<?
		if ( $edit )
		{
?>
<script type="text/javascript">
	function fnInc(sId)
	{
		var oInput = document.getElementById('bci' + sId);
		if ( oInput ) oInput.value = 1 * oInput.value + 1;
	}
	function fnDec(sId)
	{
		var oInput = document.getElementById('bci' + sId);
		if ( oInput && 1 * oInput.value > 0 ) oInput.value = 1 * oInput.value - 1;
	}
</script>

<form method="post" id="basket" action="<?= $_SERVER['PHP_SELF'] ?>">
	<input name="action" type="hidden" value=""/>
<?
		}
?>
	<table cellpadding="2" cellspacing="0" class="cart">
		<tr class="header">
			<td style="width: 5%">
				�
			</td>
			<td style="width: 45%">
				�����
			</td>
			<td style="width: 10%">
				����
			</td>
			<td style="width: 10%">
				���-��
			</td>
			<td style="width: 10%">
				���������
			</td>
			<td style="width: 10%">
<?		if ( $edit ) { ?>
				��������
<?		} else { ?>
				&nbsp;
<?		} ?>
			</td>
			<td style="width: 10%">
<?		if ( $edit ) { ?>
				�������
<?		} else { ?>
				&nbsp;
<?		} ?>
			</td>
		</tr>
<?
	$cnt = 1; $sum = 0;
	foreach ( $products as $product_id => $product_item )
	{
		$cost = $product_item['product_price']*$product_item['product_count']; $sum += $cost;
?>
		<tr>
			<td style="text-align: center">
				<?= $cnt++ ?>.
			</td>
			<td>
				<?= $product_item['product_name'] ?>
			</td>
			<td style="text-align: right">
				<?= $product_item['product_price'] ?>
			</td>
			<td style="text-align: right">
<?		if ( $edit ) { ?>
				<input id="bci<?= $product_id ?>" name="products[<?= $product_id ?>]" type="text" value="<?= $product_item['product_count'] ?>" readonly="readonly" class="quantity"/>
<?		} else { ?>
				<?= $product_item['product_count'] ?>
<?		} ?>
			</td>
			<td style="text-align: right">
				<?= $cost ?>
			</td>
			<td style="text-align: center">
<?		if ( $edit ) { ?>
				<a href="" onclick="fnInc('<?= $product_id ?>'); return false"><img src="images/plus.gif" alt="���������" title="���������"/></a> <a href="" onclick="fnDec('<?= $product_id ?>'); return false"><img src="images/minus.gif" alt="���������" title="���������"/></a>
<?		} else { ?>
				&nbsp;
<?		} ?>
			</td>
			<td style="text-align: center">
<?		if ( $edit ) { ?>
				<a href="?action=delete&amp;product_id=<?= $product_id ?>"><img src="images/delete.gif" alt="�������" title="�������"/></a>
<?		} else { ?>
				&nbsp;
<?		} ?>
			</td>
		</tr>
<?
	}
?>
		<tr>
			<td colspan="3" style="text-align: right">
				<b>����� ����� ������:</b>
			</td>
			<td colspan="2" style="text-align: right">
				<b><?= $sum ?> �.</b>
			</td>
			<td colspan="2"/>
		</tr>
		<tr>
<?
		if ( $edit )
		{
?>
			<td colspan="7" align="center">
<?
			if ( $sum < $order_limit )
			{
?>
				<input type="button" value="��������" onclick="alert('��������� ������ ��������-�������� �������������� ��� ��������� �� ����� <?= $order_limit ?> ������. ���� �� ������ �������� ����� �� ������� �����, ����������, ���������� � ���� �� ����� ���������-���������.')" class="button"/>
<?
			}
			else
			{
?>
				<input type="button" value="��������" onclick="document.forms['basket'].action.value = 'order'; document.forms['basket'].submit()" class="button"/>
<?
			}
?>			
				<input type="button" value="��������" onclick="if ( confirm('�� �������, ��� ������ �������� �������?') ) { document.forms['basket'].action.value = 'clear'; document.forms['basket'].submit() }" class="button"/>
				<input type="button" value="�����������" onclick="document.forms['basket'].action.value = 'set'; document.forms['basket'].submit()" class="button"/>
			</td>
<?
		}
		else
		{
?>
			<td colspan="7">
				<input type="button" value="���������" onclick="window.location = 'basket.php<? if ( $action === 'preview' ) print "?action=order" ?>'" class="button"/>
			</td>
<?
		}
?>
		</tr>
	</table>
<?
		if ( $edit )
		{
?>
</form>
<p class="p" style="margin-top: 20px">���� �� �� ��������� ������ ������, �� ������ ������� �� ������� � ����� ������ �������� ������� � ���������� ��� ��������. ����� ����, ��� ����������� ����� ������, � ������� ������ "+" � "-" �� ������ ������� ������ ���������� ������. ����� ����� ������� "�����������", ����� ������ ������������� ����� ������ ������. ����� ���������� � ���������� ������, ������� ������ "��������".</p>
<p class="p">�� ������ ������� ��������� ������� (������� ������ ������� �������) ���� �������� ������� ��������� (������ "��������").</p>
<p class="p"><b>��������! ������ ����� ������� ����������� �� ������� �������� ���� ������ �������� ���� �� ������� ���������� ������.</b></p>
<?
		}
		else if ( $action === 'order' )
		{
?>
<p class="p" style="margin-top: 20px">����������, ����������� ��������� ��������������� �����. ��������� ���������� ����� ������������ ������ ��� �������� ��������� � ���� � ���� ���������� � �������� ������.</p>
<?
		}
		else if ( $action === 'preview' )
		{
?>
<p class="p" style="margin-top: 20px">����������, ����������� ��������� ����������� ���� ���������� ����������, � ������ ������� ��������! ���� ���������� ���-���� ���������, ������� "���������".</p>
<?
		}
	}
	
	function PrintOrderForm($client_data, $edit = false)
	{
?>
	<div class="indent"></div>

	<form method="post" id="order" action="<?= $_SERVER['PHP_SELF'] ?>"<? if ( $edit ) print "onsubmit=\"return ( document.forms['order'].client_name.value != '' &amp;&amp; document.forms['order'].client_email.value != '' &amp;&amp; document.forms['order'].client_phone.value != '' &amp;&amp; document.forms['order'].client_address.value != '' )\"" ?>>
		<input name="action" type="hidden" value="<?= ( $edit )?'preview':'send';  ?>"/>
		<table cellpadding="3" cellspacing="0" style="width: 100%; border-collapse: collapse">
			<tr valign="top" style="height: 30px">
				<td style="width: 200px">
					<p>���<? if ( $edit ) print '<span class="require">*</span>' ?></p>
				</td>
				<td align="left">
<?		if ( $edit ) { ?>
					<input type="text" name="client_name" value="<?= $client_data['client_name'] ?>" class="text" style="width: 50%"/>
<?		} else { ?>
					<input type="hidden" name="client_name" value="<?= $client_data['client_name'] ?>"/>
					<p><?= $client_data['client_name'] ?></p>
<?		} ?>
				</td>
			</tr>
			<tr valign="top" style="height: 30px">
				<td>
					<p>E-mail<? if ( $edit ) print '<span class="require">*</span>' ?></p>
				</td>
				<td align="left">
<?		if ( $edit ) { ?>
					<input type="text" name="client_email" value="<?= $client_data['client_email'] ?>" class="text" style="width: 50%"/>
<?		} else { ?>
					<input type="hidden" name="client_email" value="<?= $client_data['client_email'] ?>"/>
					<p><?= $client_data['client_email'] ?></p>
<?		} ?>
				</td>
			</tr>
			<tr valign="top" style="height: 30px">
				<td>
					<p>���������� ��������<? if ( $edit ) print '<span class="require">*</span>' ?></p>
				</td>
				<td align="left">
<?		if ( $edit ) { ?>
					<input type="text" name="client_phone" value="<?= $client_data['client_phone'] ?>" class="text" style="width: 50%"/>
<?		} else { ?>
					<input type="hidden" name="client_phone" value="<?= $client_data['client_phone'] ?>"/>
					<p><?= $client_data['client_phone'] ?></p>
<?		} ?>
				</td>
			</tr>
			<tr valign="top" style="height: 30px">
				<td>
					<p>����� ��������<? if ( $edit ) print ' (��������) <span class="require">*</span>' ?></p>
				</td>
				<td align="left">
<?		if ( $edit ) { ?>
					<textarea name="client_address" cols="100" rows="5" style="width: 100%"><?= $client_data['client_address'] ?></textarea>
<?		} else { ?>
					<input type="hidden" name="client_address" value="<?= $client_data['client_address'] ?>"/>
					<p><?= $client_data['client_address'] ?></p>
<?		} ?>
				</td>
			</tr>
			<tr valign="top" style="height: 30px">
				<td>
					<p>����������� � ������<? if ( $edit ) print '<br/>(������� ����� ��������,<br/>������ � ��.)' ?></p>
				</td>
				<td align="left">
<?		if ( $edit ) { ?>
					<textarea name="client_comment" cols="100" rows="5" style="width: 100%"><?= $client_data['client_comment'] ?></textarea>
<?		} else { ?>
					<input type="hidden" name="client_comment" value="<?= $client_data['client_comment'] ?>"/>
					<p><?= $client_data['client_comment'] ?></p>
<?		} ?>
				</td>
			</tr>
			<tr valign="top" style="height: 30px">
				<td align="left">
					<input type="submit" value="���������" class="button"/>
				</td>
				<td/>
			</tr>
		</table>
	</form>
<?		
	}
	
	function getSearchString( $search_text, $field_name )
	{
		if ( $search_text == '' ) return '';
				
		$search_words = preg_split('/\s+/', $search_text);	
		for ( $i = 0; $i < count($search_words); $i++ )
			$search_words[$i] = $field_name." like '%$search_words[$i]%'";
			
		return join(' and ', $search_words);
	}
?>