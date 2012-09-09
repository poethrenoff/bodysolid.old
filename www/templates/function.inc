<?	
	function PrintProducts( $products_rows )
	{
		$max_product_col = 3; $products_cnt = count($products_rows);
		
		global $product_preview_images, $product_preview_picture_default, $course, $discount_limit, $discount_value;
?>

<table cellpadding="2" cellspacing="0" style="width: 100%">
<?
	    for ( $i = 0; $i < $products_cnt; $i += $max_product_col )
		{
?>
	<tr align="center" valign="top">
<?
			for ( $j = 0; $j < $max_product_col; $j++ )
			{
?>
		<td style="width: <?= round(100/$max_product_col) ?>%">
<?
				if ( $i + $j < $products_cnt )
				{
					$product_id = $products_rows[$i + $j]['product_id'];
					$product_name = $products_rows[$i + $j]['product_name'];
?>
			<p class="c"><a href="?tab=product&amp;product_id=<?= $product_id ?>"><?= $product_name ?></a></p>
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
			for ( $j = 0; $j < $max_product_col; $j++ )
			{
?>
		<td>
<?
				if ( $i + $j < $products_cnt )
				{
					$product_id = $products_rows[$i + $j]['product_id'];
					$product_name = $products_rows[$i + $j]['product_name'];
					$product_picture = $products_rows[$i + $j]['product_picture'];
					
					if ( !file_exists($_SERVER['DOCUMENT_ROOT'].$product_preview_images.$product_picture) ||
						 !is_file($_SERVER['DOCUMENT_ROOT'].$product_preview_images.$product_picture) )
						$product_picture = $product_preview_picture_default;
?>
			<a href="?tab=product&amp;product_id=<?= $product_id ?>"><img src="<?= $product_preview_images.$product_picture ?>" class="product" alt="<?= $product_name ?>" title="<?= $product_name ?>"/></a>
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
					$product_short_desc = $products_rows[$i + $j]['product_short_desc'];
				  	$product_no_discount = $products_rows[$i + $j]['product_no_discount'];
					
					if ( !$product_no_discount && $product_price > $discount_limit )
						$product_price *= $discount_value;					
?>
			<table cellpadding="2" cellspacing="0" style="width: 100%">
				<tr>
					<td style="width: 50%">
						<p class="c"><b><?= round($product_price * $course) ?> р.</b></p>
					</td>
					<td style="width: 50%" align="right">
						<a href="basket.php?action=add&amp;product_id=<?= $product_id ?>" style="margin-right: 35px"><img src="images/basket.gif" alt="Купить" title="Купить"/></a>
					</td>
				</tr>
				<tr>
					<td colspan="2">
						<p class="c"><?= $product_short_desc ?></p>
					</td>
				</tr>
			</table>
	
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

	function PrintBasket( $products, $edit = false )
	{
		global $action, $order_limit;
?>
<p class="p" style="margin-bottom: 20px">Список выбранных Вами спортивных товаров:</p> 
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
	<table cellpadding="3" cellspacing="0" style="width: 100%; border-collapse: collapse">
		<tr valign="middle" style="height: 20px">
			<td style="width: 15px; border: 1px solid #bbbbbb">
				<p class="c">№</p>
			</td>
			<td style="border: 1px solid #bbbbbb">
				<p class="c">Товар</p>
			</td>
			<td style="width: 60px; border: 1px solid #bbbbbb">
				<p class="c">Цена</p>
			</td>
			<td style="width: 60px; border: 1px solid #bbbbbb">
				<p class="c">Кол-во</p>
			</td>
			<td style="width: 60px; border: 1px solid #bbbbbb">
				<p class="c">Стоимость</p>
			</td>
			<td style="width: 60px<? if ( $edit ) print "; border: 1px solid #bbbbbb" ?>">
<?		if ( $edit ) { ?>
				<p class="c">Изменить</p>
<?		} else { ?>
				<p>&nbsp;</p>
<?		} ?>
			</td>
			<td style="width: 60px<? if ( $edit ) print "; border: 1px solid #bbbbbb" ?>">
<?		if ( $edit ) { ?>
				<p class="c">Удалить</p>
<?		} else { ?>
				<p>&nbsp;</p>
<?		} ?>
			</td>
		</tr>
<?
	$cnt = 1; $sum = 0;
	foreach ( $products as $product_id => $product_item )
	{
		$cost = $product_item['product_price']*$product_item['product_count']; $sum += $cost;
?>
		<tr valign="middle" style="height: 30px">
			<td style="border: 1px solid #bbbbbb">
				<p class="r"><?= $cnt++ ?>.</p>
			</td>
			<td style="border: 1px solid #bbbbbb">
				<p><?= $product_item['product_name'] ?></p>
			</td>
			<td style="border: 1px solid #bbbbbb">
				<p class="r"><?= $product_item['product_price'] ?></p>
			</td>
			<td style="border: 1px solid #bbbbbb">
<?		if ( $edit ) { ?>
				<input id="bci<?= $product_id ?>" name="products[<?= $product_id ?>]" type="text" value="<?= $product_item['product_count'] ?>" class="basket" readonly="readonly"/>
<?		} else { ?>
				<p class="r"><?= $product_item['product_count'] ?></p>
<?		} ?>
			</td>
			<td style="border: 1px solid #bbbbbb">
				<p class="r"><?= $cost ?></p>
			</td>
			<td<? if ( $edit ) print " style=\"border: 1px solid #bbbbbb\"" ?>>
<?		if ( $edit ) { ?>
				<p class="c"><a href="" onclick="fnInc('<?= $product_id ?>'); return false"><img src="images/plus.gif" alt="Увеличить" title="Увеличить"/></a> / <a href="" onclick="fnDec('<?= $product_id ?>'); return false"><img src="images/minus.gif" alt="Уменьшить" title="Уменьшить"/></a></p>
<?		} else { ?>
				<p>&nbsp;</p>
<?		} ?>
			</td>
			<td<? if ( $edit ) print " style=\"border: 1px solid #bbbbbb\"" ?>>
<?		if ( $edit ) { ?>
				<p class="c"><a href="?action=delete&amp;product_id=<?= $product_id ?>"><img src="images/delete.gif" alt="Удалить" title="Удалить"/></a></p>
<?		} else { ?>
				<p>&nbsp;</p>
<?		} ?>
			</td>
		</tr>
<?
	}
?>
		<tr valign="middle" style="height: 30px">
			<td colspan="3">
				<p class="r"><b>Общая сумма заказа:</b></p>
			</td>
			<td colspan="2">
				<p class="r"><b><?= $sum ?> р.</b></p>
			</td>
			<td colspan="2"/>
		</tr>		
		<tr valign="middle" style="height: 30px">
<?
		if ( $edit )
		{
?>
			<td colspan="3">
				<table cellpadding="0" cellspacing="0" style="width: 100%">
					<tr>
						<td style="width: 33%" align="left">
<?
			if ( $sum < $order_limit )
			{
?>
							<input type="button" value="Оформить" onclick="alert('Розничные заказы интернет-магазина обрабатываются при стоимости не менее <?= $order_limit ?> рублей. Если вы хотите заказать товар на меньшую сумму, пожалуйста, обратитесь в один из наших магазинов-партнеров.')" class="button"/>
<?
			}
			else
			{
?>
							<input type="button" value="Оформить" onclick="document.forms['basket'].action.value = 'order'; document.forms['basket'].submit()" class="button"/>
<?
			}
?>			
						</td>
						<td style="width: 33%" align="center">
							<input type="button" value="Очистить" onclick="if ( confirm('Вы уверены, что хотите очистить корзину?') ) { document.forms['basket'].action.value = 'clear'; document.forms['basket'].submit() }" class="button"/>
						</td>
						<td style="width: 34%" align="right">
							<input type="button" value="Пересчитать" onclick="document.forms['basket'].action.value = 'set'; document.forms['basket'].submit()" class="button"/>
						</td>
					</tr>
				</table>
			</td>
			<td colspan="4"/>
<?
		}
		else
		{
?>
			<td colspan="3" align="left">
				<input type="button" value="Вернуться" onclick="window.location = 'basket.php<? if ( $action === 'preview' ) print "?action=order" ?>'" class="button"/>
			</td>
			<td colspan="4"/>
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
<p class="p" style="margin-top: 20px">Если Вы не закончили подбор товара, Вы можете перейти из корзины в любой раздел каталога товаров и продолжить его изучение. После того, как необходимый товар выбран, с помощью кнопок "+" и "-" Вы можете указать нужное количество товара. После этого нажмите "Пересчитать", чтобы узнать окончательную сумму Вашего заказа. Чтобы приступить к оформлению заказа, нажмите кнопку "Оформить".</p>
<p class="p">Вы можете удалить отдельные позиции (крайний правый столбец таблицы) либо очистить корзину полностью (кнопка "Очистить").</p>
<p class="p"><b>ВНИМАНИЕ! Список ваших покупок сохраняется до момента закрытия окна вашего браузера либо до момента оформления заказа.</b></p>
<?
		}
		else if ( $action === 'order' )
		{
?>
<p class="p" style="margin-top: 20px">Пожалуйста, внимательно заполните нижеприведенную форму. Указанная информация будет использована только для контакта менеджера с Вами в ходе оформления и доставки заказа.</p>
<?
		}
		else if ( $action === 'preview' )
		{
?>
<p class="p" style="margin-top: 20px">Пожалуйста, внимательно проверьте оставленную Вами контактную информацию, в первую очередь телефоны! Если необходимо что-либо исправить, нажмите "Вернуться".</p>
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
					<p>ФИО<? if ( $edit ) print '<span class="require">*</span>' ?></p>
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
					<p>Контактные телефоны<? if ( $edit ) print '<span class="require">*</span>' ?></p>
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
					<p>Адрес доставки<? if ( $edit ) print ' (подробно) <span class="require">*</span>' ?></p>
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
					<p>Комментарии к заказу<? if ( $edit ) print '<br/>(удобное время доставки,<br/>сборки и пр.)' ?></p>
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
					<input type="submit" value="Отправить" class="button"/>
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