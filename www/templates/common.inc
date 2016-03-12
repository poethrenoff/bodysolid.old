<?
	require 'connect.inc';

	session_start();
    
    ini_set('default_charset', 'Windows-1251');
	
	if ( !get_magic_quotes_gpc() )
	{
		foreach ($_REQUEST as $request_key => $request_value)
		{
			if ( is_array($request_value) )
			{
				foreach ($request_value as $array_key => $array_value)
					$request_value[$array_key] = addslashes($array_value);
				$_REQUEST[$request_key] = $request_value;
			}
			else
				$_REQUEST[$request_key] = addslashes($request_value);
		}
	}

	$image_base = 'http://bodysolid.ru';

	$category_images = "/images/categories/";
	$category_picture_default = 'default.gif';

	$brand_images = "/images/brands/";
	$brand_picture_default = 'default.gif';

	$product_images = "/images/products/";
	$product_picture_default = 'default.gif';

	$product_preview_images = "/images/products/preview/";
	$product_preview_picture_default = 'default.gif';

	$product_big_images = "/images/products/big/";

	$product_additional_images = "/images/products/additional/";
	$product_additional_picture_default = 'default.gif';

	$product_additional_big_images = "/images/products/additional/big/";

	$product_instructions = "/instructions/";

	$heads_default = array ('BodySolid: силовые тренажеры, профессиональные тренажеры, свободные веса Body Solid, блочные силовые тренажеры, беговые дорожки, велотренажеры, эллиптические тренажеры',
							'BodySolid силовые тренажеры, профессиональные тренажеры, свободные веса Body Solid, блочные силовые тренажеры, беговые дорожки, велотренажеры, эллиптические тренажеры',
							'—ама€ полна€ информаци€ о тренажерах BodySolid: силовые тренажеры, профессиональные тренажеры, свободные веса Body Solid, блочные силовые тренажеры, беговые дорожки, велотренажеры, эллиптические тренажеры');

	function init_string($varname, $vardef)
	{
		if ( isset($_REQUEST[$varname]) ) 
			return (string)$_REQUEST[$varname];
		else
			return (string)$vardef;
	}  	  

	function init_integer($varname, $vardef)
	{
		if ( isset($_REQUEST[$varname]) && $_REQUEST[$varname] != '' && !preg_match("/[^0-9-]/",$_REQUEST[$varname]) ) 
			return (integer)$_REQUEST[$varname];
		else
			return (integer)$vardef;
	}  	  

	function init_float($varname, $vardef)
	{
		if ( isset($_REQUEST[$varname]) && $_REQUEST[$varname] != '' && !preg_match("/[^0-9,.]/",$_REQUEST[$varname]) ) 
			return (float)str_replace(',','.',$_REQUEST[$varname]);
		else
			return (float)str_replace(',','.',$vardef);
	}
	
	function init_array($varname, $vardef)
	{
		if ( isset($_REQUEST[$varname]) && is_array($_REQUEST[$varname]) ) 
			return (array)$_REQUEST[$varname];
		else
			return (array)$vardef;
	}

	function norm_string($str)
	{
		return htmlspecialchars(str_replace('\"','"',trim($str)), ENT_QUOTES, 'Windows-1251');
	}

	function norm_html($str)
	{
		return str_replace('\"','"',trim($str));
	}

	function norm_text($str)
	{
		return str_replace("\\\\","\\",str_replace("\'","'",str_replace('\"','"',trim($str))));
	}

	function norm_search( $text )
	{
		$text = @eregi_replace( '[[:punct:]]', ' ', to_lower_case( strip_tags( trim($text) ) ) );
		return  trim( @eregi_replace( '[[:space:]].{1,2}[[:space:]]', ' ', " $text " ) );
	}
	
	function to_lower_case( $text )
	{
		return strtr($text, "ABCDEFGHIJKLMNOPQRSTUVWXYZјЅ¬√ƒ≈®∆«» ЋћЌќѕ–—“”‘’÷„ЎўЏџ№Ёёя",
						    "abcdefghijklmnopqrstuvwxyzабвгдеЄжзиклмнопрстуфхцчшщъыьэю€");
	}

	function get_head($tab, $id)
	{
		global $heads_default;

		if ( !( $heads_row = mysql_fetch_row( mysql_query("select head_title, head_keywords, head_description from heads where head_tab = '$tab' and head_id='$id'") ) ) )
			if ( !( $heads_row = mysql_fetch_row( mysql_query("select head_title, head_keywords, head_description from heads where head_tab = '$tab' and head_id='0'") ) ) )
				return $heads_default;

		return $heads_row;
	}
	
	function get_preference($name, $default_value)
	{
		$preference_row = mysql_fetch_array( mysql_query("select * from preferences where preference_name = '$name'") );

		if ( $preference_row )
		{
			switch ( $preference_row['preference_type'] )
			{
				case 'int': case 'integer': return (integer) $preference_row['preference_value'];
				case 'bool': case 'boolean': return (boolean) $preference_row['preference_value'];
				case 'float': case 'double': case 'real': return (float) $preference_row['preference_value'];
				case 'string': return (string) $preference_row['preference_value'];
				case 'array': return (array) $preference_row['preference_value'];
				case 'object': return (object) $preference_row['preference_value'];
				default: return $preference_row['preference_value'];
			}
		}
		else
		{
			return $default_value;
		}
	}
?>