<?php
include_once($_SERVER['DOCUMENT_ROOT']."/JP_GIS/src/controller/AdapterController.class.inc");
include_once($_SERVER['DOCUMENT_ROOT']."/JP_GIS/src/util/ReportUtil.php");

class RequestListener{

	static function processPOST(){
		
	
	    if($_POST['action'] == "autoComplete"){
	        Controller::retrieveAutoCompleteItens($_POST['x'], $_POST['y'], $_POST['w'], $_POST['z']);
	    }
	    else if($_POST['action'] == "logout"){
	        SecurityUtil::unsetSession();
	    }else if($_POST['action'] == 'popUpInfo' && isset($_POST['lat']) && isset($_POST['lon'])){
		    
		    $_POST['x'] = explode(":", $_POST['x']);
		    $_POST['s'] = explode(":", $_POST['s']);
		    $str = "";
		    $i = 0;
		    
		    foreach ($_POST['x'] as $table)	{
		        $str = "";
		        if($table != ''){
		            echo "<h1>".VectorController::getLayerTitle($table)."</h1>";
		            $clause = "geom is not null ORDER BY st_distance((select st_setsrid(st_makepoint(".$_POST['lon'].",".$_POST['lat']."),900913)),geom) ASC LIMIT 1";
		            Controller::retrieveGISRecords($_POST['s'][$i],$table, $clause);
		            $str = "<br/>";
		            echo $str;
		        }
		        $i++;
		    }
		    
		}
		else if($_POST['action'] == 'featureInfo'){

			$_POST['x'] = explode(":", $_POST['x']);
			$_POST['s'] = explode(":", $_POST['s']);
			$str = "";
			$i = 0;
			
			foreach ($_POST['x'] as $table)	{
				$str = "";
				if($table != ''){
					echo "<h1>".VectorController::getLayerTitle($table)."</h1>";
					$clause = "st_intersects(geom,st_geomfromewkt('{$_POST['geocodbox']}'))";//$_POST['f']!=''?"st_intersects(geom,st_geomfromewkt('{$_POST['geocodbox']}')) and ".$_POST['f']:"st_intersects(geom,st_geomfromewkt('{$_POST['geocodbox']}'))";						
					Controller::retrieveGISRecords($_POST['s'][$i],$table, $clause);
					$str = "<br/>";
					echo $str;
				}
				$i++;
			}
			
		}else if($_POST['action'] == "queryData"){
			
			if(isset($_POST['fC']))
				Controller::retrieveForeignHomePageQueryData($_POST['y'], $_POST['z'], $_POST['k'], $_POST['data']['attributeCombo'],$_POST['data']['searchKey'],$_POST['fC'],$_POST['fV']);
			else
				Controller::retrieveHomePageQueryData($_POST['y'], $_POST['z'], $_POST['k'], $_POST['data']['attributeCombo'],$_POST['data']['searchKey']);
			
		}else if($_POST['action'] == "loginAdmin"){
			SecurityUtil::createSession($_POST['nome'], $_POST['senha']);
		}else if($_POST['action'] == "foreignQueryAttributes"){
			Controller::generateForeignQueryAttributes($_POST['y'], $_POST['z'], $_POST['k']);
		}else if($_POST['action'] == "geomFormCRUD" && isset($_POST['w'])){
			Controller::generateGeoFormFromTable($_POST['y'], $_POST['z'], $_POST['k'], $_POST['w']) ;
		}
	
	}
	
	static function processGET(){
	
	    if($_GET['x'] == "exportData"){
	        ReportUtil::generateXLSReport($_GET['y'], $_GET['z'], $_GET['k']);
	    }
	
	}
	
	static function checkRequest(){
	
		if(isset($_POST['action'])){
			self::processPOST();
		}else if(isset($_GET['x'])){
			self::processGET();
		}
		///else{
			//self::checkIndexaPage();
		//}
		
	}
	
	static function checkInfoRequest(){
	
	}
	
	static function checkNonNumericValues($list){
	
		$keys = array_keys($list);
	
		$values = array_values($list);
		
		for($i = 0; $i != count($keys); $i++){
			if(!is_numeric($values[$i])){
				if($values[$i] == '')
					$list[$keys[$i]]  = "null";
				else
					$list[$keys[$i]]  = "'".self::checkEncoding($values[$i])."'";
			}
		}
	
		return $list;
	
	
	}
	
	static function checkEncoding($values){
	
		$values = preg_replace( "/\r|\n/", "", $values );
	
		if(mb_detect_encoding($values) == 'ISO-8859-1')
			return mb_convert_encoding($values,'UTF-8', 'ISO-8859-1');
		else
			return $values;
	
	
	}
	
	static function decodeRequest($request){
	
		if(isset($request['x']))
			$request['x'] = base64_decode($request['x']);
		if(isset($request['y']))
			$request['y'] = base64_decode($request['y']);
		if(isset($request['z']))
			$request['z'] = base64_decode($request['z']);
		if(isset($request['k']))
			$request['k'] = base64_decode($request['k']);
	
		return $request;
	}
	
	static function encodeRequest($request){
	
		if(isset($request['x']))
			$request['x'] = base64_encode($request['x']);
		if(isset($request['y']))
			$request['y'] = base64_encode($request['y']);
		if(isset($request['z']))
			$request['z'] = base64_encode($request['z']);
		if(isset($request['k']))
			$request['k'] = base64_encode($request['k']);
		if(isset($request['w']))
			$request['w'] = base64_encode($request['w']);
		
		
		return $request;
	}
	
	static function processFormCrudRequest(){
	
		if(isset($_GET['fC']) && isset($_GET['fV'])){
			Controller::$isForeign = array("fC"=>$_GET['fC'],"fV"=>$_GET['fV']);
		}
		
		if(isset(DataBase::$tableWithFiles["{$_GET['y']}.{$_GET['z']}"]))
			Controller::generateFormFileFromTable($_GET['y'], $_GET['z'], $_GET['k'], isset($_GET['w'])?$_GET['w']:false) ;
		else
			Controller::generateFormFromTable($_GET['y'], $_GET['z'], $_GET['k'], isset($_GET['w'])?$_GET['w']:false) ;

		if(isset($_GET['fC']) && isset($_GET['fV'])){
			Controller::$isForeign = array("fC"=>$_GET['fC'],"fV"=>$_GET['fV']);
			HTML::generateUtilForeignVariables($_GET['fC'], $_GET['fV']);
			HTML::generateJsScript('$'."('#{$_GET['fC']}').val({$_GET['fV']});");
			
			//KL Manager Only
			if($_GET['z']=='nivel' || $_GET['z']=='subnivel'){
				
				HTML::generateJsScript("Listener.sendRequest('merge',null,null);");
				
			}	
			
		}
		
	}
	
	static function processFileRequest(){
	
		$fileId = ($_POST['w']?$_POST['w']:(DAO::retrieveIdCount($_POST['y'], $_POST['z'], $_POST['k'])+1));
			
		if (self::storeFile($_POST['z'],DataBase::$tableWithFiles["{$_POST['y']}.{$_POST['z']}"],$fileId)) {
				
				print HTML::FILE_INSERTED;
				
				unset($_POST['action']);
				$schema = $_POST['y'];
				unset($_POST['y']);
				$table = $_POST['z'];
				unset($_POST['z']);
				$pk = $_POST['k'];
				unset($_POST['k']);
				$id = $_POST['w'];
				unset($_POST['w']);
				
				$_POST[DataBase::$tableWithFiles["{$schema}.{$table}"]] = $_FILES[DataBase::$tableWithFiles["{$schema}.{$table}"]]['name'];
				
				if($id){
					
					$_POST = self::checkNonNumericValues($_POST);
					
					Controller::update($schema, $table, $_POST, $pk, $id);
				}else{
					
					$_POST = self::checkNonNumericValues($_POST);
					
					Controller::save($schema, $table, $pk, $_POST);
				}
		} else {
				print HTML::FILE_NOT_INSERTED;
		}
			
	}
	
	static function processMultiUpdateRequest(){
	
			$data  = explode(";", $_POST['x']);

			$rs = false;
			
			foreach ($data as $value){

				$aux = explode(".", $value);
				
				if(isset($aux[1]) && isset($_POST["{$aux[0]}_{$aux[1]}"])){
					
					$_POST["{$aux[0]}_{$aux[1]}"]['data'] = self::checkNonNumericValues($_POST["{$aux[0]}_{$aux[1]}"]['data']);

					$rs = AdapterController::update($aux[0], $aux[1],$_POST["{$aux[0]}_{$aux[1]}"]['data'], $_POST["{$aux[0]}_{$aux[1]}"]['k'],$_POST["{$aux[0]}_{$aux[1]}"]['data'][$_POST["{$aux[0]}_{$aux[1]}"]['k']]) ;

				}
				
			}
			
	}
	
	static function processMultiMergeRequest(){
	
			$data  = explode(";", $_POST['x']);
			
			$rst = false;
			
			foreach ($data as $value){

				$aux = explode(".", $value);
				
				if(isset($aux[1]) && isset($_POST["{$aux[0]}_{$aux[1]}"]) && isset($_POST["{$aux[0]}_{$aux[1]}"]['data'])){
					
					$_POST["{$aux[0]}_{$aux[1]}"]['data'] = self::checkNonNumericValues($_POST["{$aux[0]}_{$aux[1]}"]['data']);

					$rst = AdapterController::save($aux[0], $aux[1], $_POST["{$aux[0]}_{$aux[1]}"]['k'], $_POST["{$aux[0]}_{$aux[1]}"]['data']) ;

				}
				
			}		
			
	}
	
	static function storeFile($table,$field,$id){
		
		$uploaddir = $_SERVER['DOCUMENT_ROOT']."/JP_GIS/tmp/";
		
		$uploadfile = $uploaddir . $_FILES[$field]['name'];

		$fileExtension = self::getFileExtension($_FILES[$field]['name']);
		
		return move_uploaded_file($_FILES[$field]['tmp_name'], $uploaddir."{$table}_{$id}.$fileExtension");
		
	}
	
	static function removeFile($table,$id,$fileExtension){
	
		$uploaddir = $_SERVER['DOCUMENT_ROOT']."/JP_GIS/tmp/";
	
		$uploadfile = $uploaddir ."{$table}_{$id}.{$fileExtension}";
	
		unlink($uploadfile);
	
	}
	
	static function getFileExtension($fileName){
	
		$fileExtension = explode(".", $fileName);
		
		return $fileExtension[(count($fileExtension)-1)];
		
		
	}
	
	static function configureGeomInPOST(){
	
		if(isset($_POST['georg'])){
			$_POST['data']['usuario'] = $_SESSION['user_ufcmanager']['id'];
			$_POST['data'][$_POST['georg']] = "st_setsrid(st_geomfromewkt('GEOMETRYCOLLECTION({$_POST['geom']})') , {$_POST['epsg']})";
		}
	
	}
	

}

RequestListener::checkRequest();


?>