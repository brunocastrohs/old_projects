<?php

session_start();

class SecurityUtil{

	public static function getSessionName(){
	
	
		if(isset($_SESSION['user_web_gis_manager']) ){
			echo $_SESSION['user_web_gis_manager']['nome'];
		}else {
		//	echo "Não há login...";
		}
	
	}
		
	public static function hasSession(){
	
		
		if(isset($_SESSION['user_web_gis_manager']) || isset($_SESSION['client'])){
	
			echo '<li><a href="/JP_GIS/logout.php">Logout</a></li>';
						
		} else {
	
			echo '<li><a href="/JP_GIS/login.php">Login</a></li>';
	
		}
	
	}
	
	public static function createSession($login,$pass){

		$user = DAO::retrieveUser($login,$pass);

		if($user){

			$_SESSION['user_web_gis_manager'] = $user;

			echo true;
			
		} else {
			echo false;
		}

	}
		
	public static function checkSession(){

		if(isset($_SESSION['user_web_gis_manager'])){

			return true;

		}

		return false;
		
	}

	public static function checkPermission($contextPermissions){

		if(isset($_SESSION['user_web_gis_manager']) && isset($_SESSION['user_web_gis_manager']['permissions'])){
			
			$userPermissions = $_SESSION['user_web_gis_manager']['permissions'];

				foreach($userPermissions as $userPermission)
					if($userPermission == "ADMIN" || strstr($contextPermissions,$userPermission))
						return true;
			
		}
		
		return false;

	}
	
	public static function populatePermission(){
	
		if(isset($_SESSION['user_web_gis_manager'])){
				
			DAO::autoPopulatePermissions();
				
		}
	
	}

	public static function unsetSession(){

		if(isset($_SESSION['user_web_gis_manager']) ){
			unset($_SESSION['user_web_gis_manager']);
		}
		
		echo true;
		
	}

	public static function initSig(){
		
		echo "<script type='text/javascript'>var allowFeature = null;</script>";
		
		$user = null;
		
		extract($_SESSION);
		
		if(isset($user) && isset($user['permissions']))
			echo "<script type='text/javascript'>allowFeature = true;</script>";
		
	}
	
}
?>