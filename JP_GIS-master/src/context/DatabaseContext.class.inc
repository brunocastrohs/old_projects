<?php
require_once($_SERVER['DOCUMENT_ROOT']."/JP_GIS/src/config/DataBase.class.inc");


class DatabaseContext{

	public static $conn = null;
	
	static function openConnection(){

	    $db = DataBase::DATA_BASE;
		
	    $port = DataBase::PORT;
		
	    $host = DataBase::HOST;
		
	    $user = DataBase::USER;
		
	    $pass = DataBase::PASS;
		
		if(self::$conn == null)
			self::$conn = pg_connect("dbname= $db port=$port	host = $host user = $user password = $pass ") or die("Nao conseguiu.");
		
		return self::$conn;
	} 

	static function execute($sqlString){
	
		$result = pg_query(self::openConnection(),$sqlString);
	
		//self::close();
		
		return $result;
	}
		
	static function mergeUserAction($user,$sql){
	
		$sql = str_replace("'", "", $sql);
			
		$sql = "INSERT INTO seguranca.log (usuario,acao) VALUES($user,'$sql')";
	
		pg_query(self::openConnection(),$sql);
	
	}
	
	public function close(){
		if(self::$conn != null){
			$boolean = pg_close(self::$conn);
			self::$conn = null;
		}
		return $boolean;
	}

}
?>