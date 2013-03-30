<?
	require 'templates/common.inc';
	
	$exchange_xml = simplexml_load_file( 'http://www.cbr.ru/scripts/XML_daily.asp' );
	if ( !$exchange_xml ) exit;
	
	$dollar_xml = $exchange_xml -> xpath( 'Valute[CharCode="USD"]' );
	if ( !$dollar_xml ) exit;
	
	$value = ( string ) $dollar_xml[0] -> Value;
	$value = str_replace( ',', '.', $value );
	
	if ( is_numeric( $value ) )
		mysql_query("update preferences set preference_value = {$value} where preference_name = 'course'");
?>
