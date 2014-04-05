<?php
ini_set('memory_limit', '-1');
ini_set('max_execution_time', 300);
require_once 'PHPExcel/Classes/PHPExcel.php';
$objReader = PHPExcel_IOFactory::createReader('Excel2007');
$objReader->setReadDataOnly(true);

$objPHPExcel = $objReader->load("test2014.xlsx");
$objWorksheet = $objPHPExcel->getActiveSheet();

$highestRow = $objWorksheet->getHighestRow(); // e.g. 10
$highestColumn = $objWorksheet->getHighestColumn(); // e.g 'F'

$highestColumnIndex = PHPExcel_Cell::columnIndexFromString($highestColumn);

//connect to mysql db, if fail send an email to admin notifying something is wrong and terminate
$link = mysql_connect('baldwinserver.com', 'baldwins_max', 'lunaflare4020');
if (!$link) 
{
    $headers = 'From: Lunaflare <admin@baldwinserver.com>' . "\r\n";
    $message = "A user has attemped to access the sql server for registration, but cannot connect to server!";
    mail("MaxwellPorte@gmail.com","Server DOWN!",$message,$headers);

    echo "<script type=\"text/javascript\">
    alert(\"Registration server is down!\");
    history.back();
     </script>";
     exit();
}
mysql_select_db("baldwins_hotel_data") or die(mysql_error());
for($counter = 1; $counter <= 1; ++$counter)
{
	$objPHPExcel->setActiveSheetIndex($counter);
  	$objWorksheet = $objPHPExcel->getActiveSheet();
	$cols = array();
	$date = 
	$cols[1] = 0;
	for($col = 2; $col <= 8; ++$col) 
	{
        	$date = PHPExcel_Style_NumberFormat::toFormattedString($objWorksheet->getCellByColumnAndRow($col, 5)->getCalculatedValue(), "YYYY-MM-DD");
        	//echo "DATE" . $date . ",,,,,, ";
        	// day
        	$cols[0] = mysql_real_escape_string($objWorksheet->getCellByColumnAndRow($col, 4)->getCalculatedValue());
        	//echo $cols[0] . ", ";
        	//offset rooms occ
        	$cols[2] = mysql_real_escape_string($objWorksheet->getCellByColumnAndRow($col, 6)->getCalculatedValue());
        	//echo $cols[2] . ", ";        	
        	//rooms cleaned
        	$cols[3] = mysql_real_escape_string($objWorksheet->getCellByColumnAndRow($col, 8)->getCalculatedValue());
        	//echo $cols[3] . ", ";
        	//pm rooms cleaned
        	$cols[4] = mysql_real_escape_string($objWorksheet->getCellByColumnAndRow($col, 9)->getCalculatedValue());
        	//echo $cols[4] . ", ";
        	// rooms sold
        	$cols[5] = mysql_real_escape_string($objWorksheet->getCellByColumnAndRow($col, 10)->getCalculatedValue());
        	//echo $cols[5] . ", ";
        	//total rooms cleaned
        	$cols[6] = mysql_real_escape_string($objWorksheet->getCellByColumnAndRow($col, 11)->getCalculatedValue());
        	//echo $cols[6] . ", ";
        	// guest room carpets cleaned
        	$cols[7] = mysql_real_escape_string($objWorksheet->getCellByColumnAndRow($col, 12)->getCalculatedValue());
        	//echo $cols[7] . ", ";
        	// documented inspections.
        	$cols[8] = mysql_real_escape_string($objWorksheet->getCellByColumnAndRow($col, 13)->getCalculatedValue());
        	//echo $cols[8] . ", ";

        	$cols[9] = mysql_real_escape_string($objWorksheet->getCellByColumnAndRow($col, 15)->getCalculatedValue());
        	//echo $cols[9] . ", ";
        	$cols[10] = mysql_real_escape_string($objWorksheet->getCellByColumnAndRow($col, 16)->getCalculatedValue());
        	//echo $cols[10] . ", ";
        	$cols[11] = mysql_real_escape_string($objWorksheet->getCellByColumnAndRow($col, 17)->getCalculatedValue()*100);
        	//echo $cols[11] . ", ";

        	$cols[12] = mysql_real_escape_string($objWorksheet->getCellByColumnAndRow($col, 19)->getCalculatedValue());
        	//echo $cols[12] . ", ";
        	$cols[13] = mysql_real_escape_string($objWorksheet->getCellByColumnAndRow($col, 20)->getCalculatedValue());
        	//echo $cols[13] . ", ";
        	$cols[14] = mysql_real_escape_string($objWorksheet->getCellByColumnAndRow($col, 21)->getCalculatedValue()*100);
		//echo $cols[14] . ", ";

        	$cols[15] = mysql_real_escape_string($objWorksheet->getCellByColumnAndRow($col, 23)->getCalculatedValue());
        	//echo $cols[15] . ", ";
        	$cols[16] = mysql_real_escape_string($objWorksheet->getCellByColumnAndRow($col, 24)->getCalculatedValue());
        	//echo $cols[16] . ", ";
        	$cols[17] = mysql_real_escape_string($objWorksheet->getCellByColumnAndRow($col, 25)->getCalculatedValue()*100);
        	//echo $cols[17] . ", ";

        	$cols[18] = mysql_real_escape_string($objWorksheet->getCellByColumnAndRow($col, 27)->getCalculatedValue());
        	//echo $cols[18] . ", ";
        	$cols[19] = mysql_real_escape_string($objWorksheet->getCellByColumnAndRow($col, 28)->getCalculatedValue());
        	//echo $cols[19] . ", ";
        	$cols[20] = mysql_real_escape_string($objWorksheet->getCellByColumnAndRow($col, 29)->getCalculatedValue());
        	//echo $cols[20] . ", ";

        	$cols[21] = mysql_real_escape_string($objWorksheet->getCellByColumnAndRow($col, 31)->getCalculatedValue());
        	//echo $cols[21] . ", ";
        	$cols[22] = mysql_real_escape_string($objWorksheet->getCellByColumnAndRow($col, 32)->getCalculatedValue());
        	//echo $cols[22] . ", ";
        	$cols[23] = mysql_real_escape_string($objWorksheet->getCellByColumnAndRow($col, 33)->getCalculatedValue()*100);
        	//echo $cols[23] . ", ";

        	$cols[24] = mysql_real_escape_string($objWorksheet->getCellByColumnAndRow($col, 35)->getCalculatedValue());
        	//echo $cols[24] . ", ";
        	$cols[25] = mysql_real_escape_string($objWorksheet->getCellByColumnAndRow($col, 36)->getCalculatedValue());
        	//echo $cols[25] . ", ";
        	$cols[26] = mysql_real_escape_string($objWorksheet->getCellByColumnAndRow($col, 37)->getCalculatedValue()*100);
        	//echo $cols[26] . ", ";

        	$cols[27] = mysql_real_escape_string($objWorksheet->getCellByColumnAndRow($col, 39)->getCalculatedValue());
        	//echo $cols[27] . ", ";
        	$cols[28] = mysql_real_escape_string($objWorksheet->getCellByColumnAndRow($col, 40)->getCalculatedValue());
        	//echo $cols[28] . ", ";
        	$cols[29] = mysql_real_escape_string($objWorksheet->getCellByColumnAndRow($col, 41)->getCalculatedValue()*100);
        	//echo $cols[29] . ", ";

        	$cols[30] = mysql_real_escape_string($objWorksheet->getCellByColumnAndRow($col, 43)->getCalculatedValue());
        	//echo $cols[30] . ", ";
        	$cols[31] = mysql_real_escape_string($objWorksheet->getCellByColumnAndRow($col, 44)->getCalculatedValue());
        	//echo $cols[31] . ", ";
        	$cols[32] = mysql_real_escape_string($objWorksheet->getCellByColumnAndRow($col, 45)->getCalculatedValue()*100);
        	//echo $cols[32] . ", ";

        	$cols[33] = mysql_real_escape_string($objWorksheet->getCellByColumnAndRow($col, 47)->getCalculatedValue());
        	//echo $cols[33] . ", ";
        	$cols[34] = mysql_real_escape_string($objWorksheet->getCellByColumnAndRow($col, 48)->getCalculatedValue());
        	//echo $cols[34] . ", ";
        	$cols[35] = mysql_real_escape_string($objWorksheet->getCellByColumnAndRow($col, 49)->getCalculatedValue()*100);
        	//echo $cols[35] . ", ";

        	$cols[36] = mysql_real_escape_string($objWorksheet->getCellByColumnAndRow($col, 51)->getCalculatedValue());
        	//echo $cols[36] . ", ";
        	$cols[37] = mysql_real_escape_string($objWorksheet->getCellByColumnAndRow($col, 52)->getCalculatedValue());
        	//echo $cols[37] . ", ";
        	$cols[38] = mysql_real_escape_string($objWorksheet->getCellByColumnAndRow($col, 53)->getCalculatedValue()*100);
        	//echo $cols[38] . ", ";

        	$cols[39] = mysql_real_escape_string($objWorksheet->getCellByColumnAndRow($col, 55)->getCalculatedValue());
        	//echo $cols[39] . ", ";
        	$cols[40] = mysql_real_escape_string($objWorksheet->getCellByColumnAndRow($col, 56)->getCalculatedValue());
        	//echo $cols[40] . ", ";
        	$cols[41] = mysql_real_escape_string($objWorksheet->getCellByColumnAndRow($col, 57)->getCalculatedValue() * 100);
        	///echo $cols[41] . ", ";

        	$cols[42] = mysql_real_escape_string($objWorksheet->getCellByColumnAndRow($col, 59)->getCalculatedValue());
        	//echo $cols[42] . ", ";
        	$cols[43] = mysql_real_escape_string($objWorksheet->getCellByColumnAndRow($col, 60)->getCalculatedValue());
        	//echo $cols[43] . ", ";
        	$cols[44] = mysql_real_escape_string(round($objWorksheet->getCellByColumnAndRow($col, 61)->getCalculatedValue(),0,PHP_ROUND_HALF_UP));
        	//echo $cols[44] . ", ";

        	$cols[45] = mysql_real_escape_string($objWorksheet->getCellByColumnAndRow($col, 63)->getCalculatedValue());
        	//echo $cols[45] . ", ";
        	$cols[46] = mysql_real_escape_string($objWorksheet->getCellByColumnAndRow($col, 64)->getCalculatedValue());
        	//echo $cols[46] . ", ";
        	$cols[47] = mysql_real_escape_string($objWorksheet->getCellByColumnAndRow($col, 65)->getCalculatedValue()*100);
        	//echo $cols[47] . ", ";

        	$cols[48] = mysql_real_escape_string($objWorksheet->getCellByColumnAndRow($col, 67)->getCalculatedValue());
        	//echo $cols[48] . ", ";
        	$cols[49] = mysql_real_escape_string($objWorksheet->getCellByColumnAndRow($col, 68)->getCalculatedValue());
        	//echo $cols[49] . ", ";
        	$cols[50] = mysql_real_escape_string($objWorksheet->getCellByColumnAndRow($col, 69)->getCalculatedValue()*100);
        	//echo $cols[50] . ".................... ";


        	$sql = "INSERT INTO `MLB_Test_Read_2013`(`Date`, `Day_Of_Week`, `Offset_Rooms_Occupied`, `AM_Rooms_Cleaned`, `PM_Rooms_Cleaned`, `Rooms_Sold`, `Total_Rooms_Cleaned`, `Guestroom_Carpets_Cleaned`,
      			`Documented_Inspections_Completed`, `AM_Room_Attendants_Hours_Paid`, `AM_Room_Attendants_Standard_Hours`, `AM_Room_Attendants_Percent_Performance`, `PM_Room_Attendants_Hours_Paid`, `PM_Room_Attendants_Standard_Hours`,
      			`PM_Room_Attendants_Percent_Performance`, `AM_Housemen_Hours_Paid`, `AM_Housemen_Standard_Hours`, `AM_Housemen_Percent_Performance`, `Carpet_Care_Hours_Paid`, `Carpet_Care_Standard_Hours`,
      			`Carpet_Care_Percent_Performance`, `AM_Lobby_Attendant_Hours_Paid`, `AM_Lobby_Attendant_Standard_Hours`, `AM_Lobby_Attendant_Percent_Performance`, `PM_Lobby_Attendant_Hours_Paid`, `PM_Lobby_Attendant_Standard_Hours`,
      			`PM_Lobby_Attendant_Percent_Performance`, `Public_Area_Attendant_Hours_Paid`, `Public_Area_Attendant_Standard_Hours`, `Public_Area_Attendant_Percent_Performance`, `Laundry_Attendant_Hours_Paid`, `Laundry_Attendant_Standard_Hours`,
      			`Laundry_Attendant_Percent_Performance`, `AM_Room_Coordinator_Hours_Paid`, `AM_Room_Coordinator_Standard_Hours`, `AM_Room_Coordinator_Percent_Performance`, `PM_Floor_Supervisor_Hours_Paid`, `PM_Floor_Supervisor_Standard_Hours`,
      			`PM_Floor_Supervisor_Percent_Performance`, `Director_And_Two_Managers_Hours_Paid`, `Director_And_Two_Managers_Standard_Hours`, `Director_And_Two_Managers_Percent_Performance`, `Overtime_Hours_Paid`, `Overtime_Standard_Hours`,
      			`Overtime_Percent_Performance`, `Total_Labor_Hours_Hours_Paid`, `Total_Labor_Hours_Standard_Hours`, `Total_Labor_Hours_Percent_Performance`,`Total_Labor_Cost_Hours_Paid`,`Total_Labor_Cost_Standard_Hours`,
      			`Total_Labor_Cost_Percent_Performance`)
    	   	VALUES('".$date."','".$cols[0]."',$cols[2],$cols[3],$cols[4],$cols[5],$cols[6],$cols[7],$cols[8],$cols[9],$cols[10],$cols[11],$cols[12],$cols[13],$cols[14],$cols[15],$cols[16],$cols[17],$cols[18],$cols[19],$cols[20],
      			$cols[21],$cols[22],$cols[23],$cols[24],$cols[25],$cols[26],$cols[27],$cols[28],$cols[29],$cols[30],$cols[31],$cols[32],$cols[33],$cols[34],$cols[35],$cols[36],$cols[37],$cols[38],$cols[39],$cols[40],
      			$cols[41],$cols[42],$cols[43],$cols[44],$cols[45],$cols[46],$cols[47],$cols[48],$cols[49],$cols[50])";
  		echo $sql . "....................................... ";
  		mysql_query($sql);
	}
}
?>

    <html xmlns="http://www.w3.org/1999/xhtml">
    <head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title>Insertion Successful</title>
    </head>

    <center>
    <h3>It Worked!</h3>
    </center>

    </body>
    </html>