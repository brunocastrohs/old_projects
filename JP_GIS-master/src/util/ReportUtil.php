<?php
include_once($_SERVER['DOCUMENT_ROOT']."/JP_GIS/src/dao/DAO.class.inc");

class ReportUtil{

    static function generateXLSReport($schema, $table, $params){
        
        $cols = DAO::generateSelectColumns(DAO::retrieveColumns($schema, $table, ""));
        
        $headers = DAO::retrieveTitles($schema, $table, $cols);
        
        $data = self::parseRecordToData(DAO::retrieveReportData($schema, $table, $cols, $params));
        
        $cols = explode(',',$cols);
        
        self::generateXLS($cols,$headers,$data);
        
    }
    
    static function parseRecordToData($records){
        
        $data = array();
        
        $flag = 0;
        
        foreach ($records as $row){
            foreach ($row as $e){
                $data[$flag][$e['column']] = $e['value'];
                //		echo "$flag - {$e['column']} - {$e['value']} ||";
            }
            //	echo "<br/>";
            $flag++;
        }
        
        return $data;
        
    }
    
    static function generateXLS($cols,$headers,$data){
        
        $fp = fopen($_SERVER['DOCUMENT_ROOT'] . "/JP_GIS/report/data.csv","wb");
        
        $isFirst = true;
        
        foreach ($cols as $col){
            if($headers[$col]!='Gid' && $headers[$col]!='Geom'){
               if($isFirst)
                    fwrite($fp,$headers[$col]);
               else
                    fwrite($fp,";".$headers[$col]);
              $isFirst=false;
            }
        }
        
        fwrite($fp,"\n");
        
        $isFirst = true;
        
        foreach ($data as $e){
            foreach ($cols as $col){
                
                if($headers[$col]!='Gid' && $headers[$col]!='Geom'){
                    if($isFirst){
                        fwrite($fp,$e[$col]);
                    }
                    else{
                        fwrite($fp,";".$e[$col]);
                    }
                    
                    $isFirst=false;
                }
            }
            $isFirst=true;
            fwrite($fp,"\n");
        }
        fclose($fp);
        
        header('Content-disposition: attachment; filename=data.csv');
        header('Content-type: application/vnd.ms-excel');
        
        // The PDF source is in original.pdf
        readfile($_SERVER['DOCUMENT_ROOT'] . "/JP_GIS/report/data.csv");
        
    }

}

?>