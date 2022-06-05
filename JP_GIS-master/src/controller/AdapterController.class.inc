<?php
include_once($_SERVER['DOCUMENT_ROOT']."/JP_GIS/src/controller/Controller.class.inc");
include_once($_SERVER['DOCUMENT_ROOT']."/JP_GIS/src/controller/VectorController.class.inc");

class AdapterController extends Controller{

	static function generateNavBar(){
	
		//here
		$searchComponent = self::getSearchComponent();
	
		$downloadComponent = self::getDownloadComponent();
		
		$loginComponent = self::getLoginComponent();
	
		$listComponent = self::getListComponent();
		
		//done
		$siderBar = HTML::createSideBarMenu($searchComponent, $listComponent, $downloadComponent.' '.$loginComponent);
	
		//here
		$navBar = self::getNavBarComponent();
	
		$header = HTML::createHeader('SIG-Joaquin Pires', " ", $navBar?$navBar." ".$siderBar:$siderBar);
	
		echo $header;
	}
	
	static function getNavBarComponent(){
	
		$list = 
		//SecurityUtil::checkSession()?array( array('text'=>"Pesquisa",'link'=>"#"), array('text'=>"Cadastro",'link'=>"#")):
		array();
	
		return HTML::createNavigationMenu(HTML::createNavigationItens($list));
	
	}
	
	static function getSearchComponent(){
	
		HTML::createSideBarSeachComponent('pesq', 'onclick=""');
	
		return "";
	
	}
	
	static function getLoginComponent(){
	
		$loginItens ="";
	
	
		if(!SecurityUtil::checkSession()){
			$loginItens  = HTML::createSideBarFormComponentItem("text","Email", "nome", "");
			$loginItens .= HTML::createSideBarFormComponentItem("password","Senha", "senha", "");
			$loginItens .= HTML::createSideBarFormComponentItem("button","Login", "", "Listener.login()");
		}else
			$loginItens = HTML::createSideBarFormComponentItem("button","Logout", "", "Listener.logout()");
			
			return HTML::createSideBarFormComponent($loginItens);
	}
	
	static function getDownloadComponent(){
	    
	    $miscItens = HTML::createSideBarFormComponentItem("button","Material Acadêmico - PDF", "", "Listener.openDialogWindow('Downloads','/JP_GIS/tmp/index.php','',true)");
	    
	    $miscItens .= HTML::createSideBarFormComponentItem("button","Mapas Temáticos - PDF", "", "Listener.openDialogWindow('Downloads','/JP_GIS/maps/index.php','',true)");
	    
	    $geoserverURL = VectorController::$geoserverHost."/web/wicket/bookmarkable/org.geoserver.web.demo.MapPreviewPage";
	    
	    $miscItens .= HTML::createSideBarFormComponentItem("button","Exportar Dados", "", "window.open('$geoserverURL', '_blank');");
	    
	    $miscItens .= HTML::createSideBarFormComponentItem("button","Manual", "", "window.open('/JP_GIS/manual/manual.pdf', '_blank');");
	    
	    //$miscItens .= HTML::createfo;
	    
	    return HTML::createSideBarFormComponent($miscItens);
	    
	}
	
	static function getListComponent(){
	
		$title = HTML::createIMG("/JP_GIS/images/logo.png", '', "class='image featured'").HTML::createTitle("Mapas Temáticos", "1").HTML::createDescription("Clique nas caixas para visualizar os mapas.", "");
		$itensListComponent  = VectorController::getLayerSwitcher();
		$listComponent = HTML::createSideBarMenuList($title, $itensListComponent);
		return $listComponent;
	
	}
	
	static function generateHomePage(){
	
    		if(SecurityUtil::checkSession()){
    		    
    		    self::generateHeader();
    		    
    		    self::generateNavBar();
    		    
    		    VectorController::initGIS();
    		    
    		    self::generateFooter();
    		    
    		    
    		}else {
    		    self::generateLoginHeader();
    		    
    		    self::generateLoginPage();
    		    
    		    self::generateLoginFooter();
    		}
		
	}

	static function generateHeader(){
	    
	    echo "
        	<!DOCTYPE HTML>
        	<html>
        	<head>
        	<title>Joaquim Pires GIS</title>
            <link rel='icon' type='image/x-icon' href='/JP_GIS/images/favicon.ico'>
        	<meta charset='utf-8' />
        	<meta name='viewport' content='width=device-width, initial-scale=1' />
        	<link rel='stylesheet' href='/JP_GIS/styles/global.css' />
        	<script src='/JP_GIS/scripts/jquery.min.js'></script>
        	<script src='/JP_GIS/scripts/skel.min.map.js'></script>
        	<script src='/JP_GIS/scripts/panel.js'></script>
        	<script src='/JP_GIS/scripts/main.js'></script>
        	<script src='/JP_GIS/scripts/listener.js'></script>

        	        
        	<script>var printCapabilities = null; var google = null;var hasD=false;var hasS=false;var hasU=false;</script>
       
        	<script type='text/javascript' src='/JP_GIS/scripts/openlayers/OpenLayers.js'></script>
        	<link rel='stylesheet' type='text/css' href='/JP_GIS/scripts/openlayers/theme/default/style.css'/>
        	<script type='text/javascript' src='/JP_GIS/scripts/vectorStyle.js'></script>
        	<script type='text/javascript' src='/JP_GIS/scripts/vectorManager.js'></script>
        	<script type='text/javascript' src='/JP_GIS/scripts/vectorControls.js'></script>
        	<script type='text/javascript' src='/JP_GIS/scripts/vectorAction.js'></script>
        	</head>
        	<body>
        ";
	    
	    
	}
	
	static function generateFooter(){
	    
	    echo "<div id='modalWindow'>
                <div class='modal-content'>
                    <div class='header'><h2 id='modalWindowTitle'></h2></div>
                        <div id='modalWindowContent' class='copy'>
</div><div id='modalWindowButtons' class='cf footer'><a href='#' class='btn'>Fechar</a></div></div><div class='overlay'></div></div>";
	    echo "</body></html>";
	    
	    
	}
	
	static function generateLoginHeader(){
	    
	    echo "<!DOCTYPE HTML>
                <html style='height:100%;'>
                	<head>
                		<title>Joaquim Pires GIS</title>
                		<meta http-equiv='content-type' content='text/html; charset=utf-8' />
                		<meta name='description' content='' />
                		<meta name='keywords' content='' />
                		<link rel='shortcut icon' href='/JP_GIS/images/header.jpg' />
                		<!--[if lte IE 8]><script src='/JP_GIS/styles/ie/html5shiv.js'></script><![endif]-->
                		<script src='/JP_GIS/scripts/jquery.min.js'></script>
                		<script src='/JP_GIS/scripts/jquery.scrolly.min.js'></script>
                		<script src='/JP_GIS/scripts/skel.min.js'></script>
                		<script src='/JP_GIS/scripts/listener.js'></script>
                        <script src='/JP_GIS/scripts/init.js'></script>
                		<noscript>
                			<link rel='stylesheet' src='/JP_GIS/styles/skel.css' />
                			<link rel='stylesheet' src='/JP_GIS/styles/style.css' />
                			<link rel='stylesheet' src='/JP_GIS/styles/style-desktop.css' />
                		</noscript>
                		<!--[if lte IE 8]><link rel='stylesheet' src='/JP_GIS/styles/ie/v8.css' /><![endif]-->
                		<!--[if lte IE 9]><link rel='stylesheet' src='/JP_GIS/styles/ie/v9.css' /><![endif]-->
                </head>
                <body onload='' style='height:100%;'>";
	    
	    
	}
	
	static function generateLoginPage(){
	    
	    echo "
                <nav id='nav'>
                    <div id='preloader'></div>
                    <div id='messageBox'></div>
                </nav>
                    <div class='wrapper style4' style='height:100%;'>
                    <article id='adminArea' class='container small'>
                    <header><h2>Bem vindo ao SIG de Joaquim Pires</h2></header>
                    <div class='row'><div class='12u'><form name='adminForm' id='adminForm' >
                        <div><div class='12u'></div>
                         <div class='row'><div class='12u'><input type='text' name='nome' id='nome' value ='' placeHolder='Nome'  /></div></div>
                         <div class='row'><div class='12u'><input type='password' name='senha' id='senha' value ='' placeHolder='Senha'  /></div></div>
                         <div class='row double'><div class='12u'><ul class='actions'><li><input type='button' onclick='Listener.login()' value='Logar'/>
                         </li></ul></div></div></div></form></div></div><footer><ul id='copyright'><li>&copy; Prefeitura de Joaquim Pires. Todos os direitos reservados.</li></ul>
                         </footer></article></div><div id='modalWindow'><div class='modal-content'><div class='header'><h2 id='modalWindowTitle'></h2></div><div id='modalWindowContent' class='copy'></div>
                         <div id='modalWindowButtons' class='cf footer'><a href='#' class='btn'>Fechar</a></div></div><div class='overlay'>
                    </div></div>";
	    
	}
	
	static function generateLoginFooter(){
	    
	    echo "<div id='modalWindow'><div class='modal-content'><div class='header'><h2 id='modalWindowTitle'></h2></div><div id='modalWindowContent' class='copy'></div><div id='modalWindowButtons' class='cf footer'><a href='#' class='btn'>Fechar</a></div></div><div class='overlay'></div></div></body></html>";
	    
	    
	}
	
	static function scanDownloadDirectory(){

	    $files = scandir($_SERVER['DOCUMENT_ROOT']."/JP_GIS/tmp/");
	    
	    $content = "";
	    
	    foreach ($files as $e)
	        $content .=  $e != '..' && $e != '.' && $e != 'index.php'?HTML::createListItem( HTML::createFileHREF($e, "/JP_GIS/tmp/$e", ""),"")."<br/>":"";
	    
	        echo HTML::createList($content == ""?"Não há produtos para download...":$content, "style=''");    
	          
	}
	
	static function scanMapsDirectory(){
	    
	    $files = scandir($_SERVER['DOCUMENT_ROOT']."/JP_GIS/maps/");
	    
	    $content = "";
	    
	    foreach ($files as $e)
	        $content .=  $e != '..' && $e != '.' && $e != 'index.php'?HTML::createListItem( HTML::createFileHREF($e, "/JP_GIS/maps/$e", ""),"")."<br/>":"";
	        
	    echo HTML::createList($content == ""?"Não há produtos para download...":$content, "style=''");
	        
	}
	
}

?>