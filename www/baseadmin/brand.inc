<?
  $id = init_integer('id',0);
        
  $error = '';
  
  switch ( $action )
  {
    case 'delete':  // �������� ������
      list($cnt) = mysql_fetch_row(mysql_query("select count(*) from products where brand_id = '$id'"));
      if (!$cnt)
      {
        mysql_query("delete from brands where brand_id = '$id'");
      }
      else $error = "������ ������� ����� � ��������!";
	  break;

    case 'addsave': // ���������� ������������ ������
    case 'editsave': // ���������� ������������� ������
      $new_name = init_string('new_name',''); $new_name = norm_string($new_name);
      $new_country = init_string('new_country',''); $new_country = norm_string($new_country);

      if ( $new_name )
      {
        if ( $action == 'editsave' )	
        {
          mysql_query("update brands set brand_name = '$new_name',
                                         brand_country = '$new_country'
                       where brand_id = '$id'");
        }
        else
        {
          mysql_query("insert into brands (brand_name, brand_country)
                                   values ('$new_name', '$new_country')");
          $id = mysql_insert_id();

          $action = 'edit';
        }
      }
      else $error = "������ ��������� ����� ��� ��������!";
       
    case 'add':  // ���������� ������
    case 'edit':  // �������������� ������
      if ( $action == 'addsave' ) $action = 'add';
      if ( $action == 'editsave' ) $action = 'edit';
      
      if ( $action == 'edit' )
      {
        $brand_sql = mysql_query("select * from brands where brand_id = '$id'");
        $brand = mysql_fetch_array($brand_sql);
            
        $brand_id = $brand['brand_id'];
        $brand_name = $brand['brand_name'];
        $brand_country = $brand['brand_country'];
      }
      else
      {
        $brand_id = 0;
        $brand_name = $brand_country = '';
      }

      PrintHeader( "<p><a href=\"index.php?tab=brand\">������</a> - ".( ( $action == 'edit' )?$brand_name:"���������� ������" )."</p>" );

      if ( $error ) { ?>    	  
    <p class="baseerror"><?= $error ?></p>
<? } ?>
    <form id="form" name="form" method="post" onsubmit="return (form.new_name.value.length>0)">
      <input type="hidden" name="id" value="<?= $id ?>"/>
      <input type="hidden" name="action" value="<?= ( $action == 'edit' )?'editsave':'addsave' ?>"/>
      <input type="hidden" name="tab" value="brand"/>
      <table width="100%" cellspacing="0" cellpadding="2" border="0" style="margin-top: 10px">
        <tr>
          <td valign="top" style="width: 100px">
            <p>��������</p>
          </td>
          <td valign="top" style="width: 300px">
            <input type="text" class="text" name="new_name" style="width: 300px" value="<?= $brand_name ?>"/>
          </td>
          <td valign="top" style="width: 200px" align="right">
            <input type="submit" class="button" style="width: 120px" value="���������"/>
          </td>
          <td rowspan="2" align="center" valign="top">
        	&nbsp;
          </td>
        </tr>
        <tr>
          <td valign="top">
            <p>������</p>
          </td>
          <td valign="top" colspan="2">
            <p><input type="text" class="text" name="new_country" style="width: 300px" value="<?= $brand_country ?>"/></p>
          </td>
        </tr>
      </table>
    </form>
  </body>
</html>
<?    
            exit;
  }

  PrintHeader( "<p>������</p>" );
  
  if ( $error ) { ?>    	  
    <p class="baseerror"><?= $error ?></p>
<? } ?>
    <form id="form" name="form" action="index.php" method="post">
      <input type="hidden" name="id" value=""/>
      <input type="hidden" name="action" value=""/>
      <input type="hidden" name="tab" value="brand"/>
      <table width="100%" cellspacing="0" cellpadding="0" border="0" style="margin-top: 10px">
        <tr>
<?    
  $brands_sql = mysql_query("select brands.brand_id, brands.brand_name,
  		                            count(products.product_id) as products_cnt
  		                         from brands left join products
  		                         on brands.brand_id = products.brand_id
                                 group by brands.brand_id
                                 order by brands.brand_name");
    
  $brands_cnt = mysql_num_rows($brands_sql);    
    
    for ( $col = 0; $col < $max_brand_cols; $col++ )
    {
?>
          <td width="<?= round(100/$max_brand_cols)."%" ?>">
            <table width="100%" cellspacing="0" cellpadding="1" border="0">
<?
      for ( $row = 0; $row * $max_brand_cols < $brands_cnt; $row++ )
      {
      if ( $brands = mysql_fetch_array($brands_sql) )
      {
        $brand_id = $brands['brand_id'];
        $brand_name = $brands['brand_name'];
        $products_cnt = $brands['products_cnt'];
?>
              <tr>
                <td width="30" align="center">
<?  if ( !$products_cnt ) { ?>
                  <input type="button" class="ibutton" value="" style="background-image: url(../images/admin/buttonminus.gif);" onclick="if (confirm('������� ����� \'<?= str_replace("'","\\'",$brand_name) ?>\'?')) { action.value='delete'; form.id.value='<?= $brand_id ?>'; submit(); }" title="�������"/>
<?  } else { ?>
                  &nbsp;
<?  } ?>
                </td>
                <td>
                  <p><a href="index.php?tab=brand&action=edit&id=<?= $brand_id ?>"><?= $brand_name ?></a></p>
                </td>
              </tr>
<?
      }
      else
      {
?>
              <tr>
                <td>
                  &nbsp;
                </td>
                <td>
                  &nbsp;
                </td>
              </tr>
<?
      }
    }
?>
            </table>
          </td>
<?
  }
?>
        </tr>
        <tr>
          <td colspan="<?= $max_brand_cols ?>">
            <table width="100%" cellspacing="0" cellpadding="1" border="0">
              <tr>
                <td width="30" align="center">
                  <input type="button" class="ibutton" value="" style="background-image: url(../images/admin/buttonplus.gif);" onclick="form.action.value='add'; submit();" title="��������"/>
                </td>
                <td>
                  &nbsp;
                </td>
              </tr>
            </table>
          </td>
        </tr>
      </table>
    </form>
