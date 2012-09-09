<?
  require "../templates/common.inc";
  
  $tab = init_string('tab','group');
  $action = init_string('action','');
  
  $max_product_list = 20;

  $max_brand_cols = 5;

?>
<html>
  <head>
    <title>"БодиСолид" :: Администрирование каталога</title>
    <link rel="stylesheet" href="../styles/admin.css" type="text/css"/> 
    <meta http-equiv="Content-Type" content="text/html; charset=windows-1251"/>
  </head>
  <body>
<?
    switch ( $tab )
    {
      case 'brand':     ////////////////////////////////////////////////////////// Бренды
      	include('brand.inc');
        break;

      case 'picture':   ////////////////////////////////////////////////////////// Картиники
      	include('picture.inc');
        break;

      case 'product':   ////////////////////////////////////////////////////////// Товары
      	include('product.inc');
  	    break;
      
      case 'category':  ////////////////////////////////////////////////////////// Категории товаров
      	include('category.inc');
        break;
      
      default:   ///////////////////////////////////////////////////////////////// Группы товаров
      	include('group.inc');
    }
?>
  </body>
</html>
<?
  function PrintHeader( $nav_str )
  {
    global $tab;
?>
    <table width="100%" cellspacing="0" cellpadding="3" style="border: 1px solid #92BA85">
      <tr>
        <td width="7%" valign="top" bgcolor="<?= ( $tab == 'product' || $tab == 'category' || $tab == 'group' || $tab == 'picture' )?'#F1F9F0':'#E3F3E1' ?>" style="border-right: 1px solid #92BA85<?= ( $tab != 'product' && $tab != 'category' && $tab != 'group' && $tab != 'picture')?'; border-bottom: 1px solid #92BA85':'' ?>">
          <p class="c"><a href="index.php?tab=group">Товары</a></p>
        </td>
        <td width="7%" valign="top" bgcolor="<?= ( $tab == 'brand' )?'#F1F9F0':'#E3F3E1' ?>" style="border-right: 1px solid #92BA85<?= ( $tab != 'brand' )?'; border-bottom: 1px solid #92BA85':'' ?>">
          <p class="c"><a href="index.php?tab=brand">Бренды</a></p>
        </td>
        <td width="83%"  valign="top" bgcolor="#E3F3E1" style="border-bottom: 1px solid #92BA85">
          <p class="h">Интернет-магазин "БодиСолид" (администрирование базы товаров)</p>
        </td>
      </tr>
      <tr>
        <td bgcolor="#F1F9F0" align="right" colspan="3">
          <?= $nav_str."\n" ?>
        </td>
      </tr>
    </table>
<?    		  
  }
?>