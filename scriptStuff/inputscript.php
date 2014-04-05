<?php
require_once './Excel/reader.php';
require_once './Excel/date.php';
$data = new Spreadsheet_Excel_Reader();
$data->setOutputEncoding('utf-8');
$data->read('testExcel.xls');

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

for($x = 1; $x<=count($data->sheets[0]["cells"]); $x++)
{
	$date = $data->sheets[0]["cells"][$x][2];
	echo $date;
	$date = ExcelToPHP($date);
	$dayofweek = $data->sheets[0]["cells"][$x][1];
	$offset_rooms_occupied = $data->sheets[0]["cells"][$x][3];
	$am_rooms_cleaned = $data->sheets[0]["cells"][$x][4];
	$pm_rooms_cleaned = $data->sheets[0]["cells"][$x][5];
	$rooms_sold = $data->sheets[0]["cells"][$x][6];
	$total_rooms = $data->sheets[0]["cells"][$x][7];
	$guestroom_carpets_cleaned = $data->sheets[0]["cells"][$x][8];
	$documented_inspections_completed = $data->sheets[0]["cells"][$x][9];
	$am_room_attendants = $data->sheets[0]["cells"][$x][10];
	$pm_room_attendants = $data->sheets[0]["cells"][$x][11];
	$am_housemen = $data->sheets[0]["cells"][$x][12];
	$pm_housemen = $data->sheets[0]["cells"][$x][13];
	$am_lobby_attendant = $data->sheets[0]["cells"][$x][14];
	$pm_lobby_attendant = $data->sheets[0]["cells"][$x][15];
	$public_area_attendant = $data->sheets[0]["cells"][$x][16];
	$laundry_attendant = $data->sheets[0]["cells"][$x][17];
	$am_room_coordinator = $data->sheets[0]["cells"][$x][18];
	$pm_floor_supervisor = $data->sheets[0]["cells"][$x][19];
	$director_and_two_managers = $data->sheets[0]["cells"][$x][20];
	$overtime_hours_paid = $data->sheets[0]["cells"][$x][21];
	$sql = "INSERT INTO `MLB_Test_Input`(`Date`, `Day_Of_Week`, `Offset_Rooms_Occupied`, `AM_Rooms_Cleaned, `PM_Rooms_Cleaned`, `Rooms_Sold`, `Total_Rooms_Cleaned`, `Guestroom_Carpets_Cleaned`, `Documented_Inspections_Completed`, `AM_Room_Attendants_Hours_Paid`, `PM_Room_Attendants_Hours_Paid`, `AM_Housemen_Hours_Paid`, `PM_Housemen_Hours_Paid`, `AM_Lobby_Attendant_Hours_Paid`, `PM_Lobby_Attendant_Hours_Paid`, `Public_Area_Attendant_Hours_Paid`, `Laundry_Attendant_Hours_Paid`, `AM_Room_Coordinator_Hours_Paid`, `PM_Floor_Supervisor_Hours_Paid`, `Director_And_Two_Managers_Hours_Paid`,`Overtime_Hours_Paid
	VALUES($date, $dayofweek, $offset_rooms_occupied, $am_rooms_cleaned, $pm_rooms_cleaned, $rooms_sold, $total_rooms, $guestroom_carpets_cleaned, $documented_inspections_completed, $am_room_attendants, $pm_room_attendants, $am_housemen, $pm_housemen, $am_lobby_attendant, $pm_lobby_attendant, $public_area_attendant, $laundry_attendant, $am_room_coordinator, $pm_floor_supervisor, $director_and_two_managers, $overtime_hours_paid )";
	echo $sql;
	echo "\n";
	mysql_query($sql);
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