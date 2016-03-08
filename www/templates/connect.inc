<?
	@mysql_connect('localhost','bodysolid','bodysolid') or die('<center>'.mysql_error().'<center>');
	@mysql_select_db('bodysolid') or die('<center>'.mysql_error().'<center>');
	@mysql_query('set names cp1251');
?>