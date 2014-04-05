<?php
	
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

	$counter = 0;
	$rooms_avail = 310;
	$am_gra = 16.65;
	$pm_gra = 13;
	$laundry_attendant = 49;
	$GRA = 8;
	$GRA_TWO = 7.5;
	
	$low_rooms_occupied = 0;
	$high_rooms_occupied = 0;
	$low_occupancy_percent = 0;
	$high_occupancy_percent = 0;
	$am_room_attendants = 0;
	$pm_room_attendants = 0;
	$am_housemen = 0;
	$pm_housemen = 0;
	$am_lobby_attendant = 0;
	$pm_lobby_attendant = 0;
	$public_area_attendants = 0;
	$laundry_attendants = 0;
	$room_coordinator = 0;
	$pm_floor_supervisor = 0;
	$director_two_managers = 0;
	$total_hours_per_day = 0;
	$total_dollars_per_day = 0;
	
	$sql = "INSERT INTO `MLB_Labor_Stds_2013`(`Rooms_Occupied_Low`, `Rooms_Occupied_High`, `Occupancy_Percent_Low`, `Occupancy_Percent_High`, `AM_Room_Attendants`, `PM_Room_Attendants`, `AM_Housemen`, `PM_Housemen`, `AM_Lobby_Attendant`, `PM_Lobby_Attendant`, `Public_Area_Attendant`, `Laundry_Attendant`, `AM_Room_Coordinator`, `PM_Floor_Supervisor`, `Director_And_Two_Managers`, `Total_Hours_Per_Day`, `Total_Dollars_Per_Day`) 
	VALUES($low_rooms_occupied, $high_rooms_occupied, $low_occupancy_percent, $high_occupancy_percent, $am_room_attendants, $pm_room_attendants, $am_housemen, $pm_housemen, $am_lobby_attendant, $pm_lobby_attendant, $public_area_attendants, $laundry_attendants, $room_coordinator, $pm_floor_supervisor, $director_two_managers, $total_hours_per_day, $total_dollars_per_day)";
	
	$result = mysql_query($sql) or die(mysql_error());
		
	$low_rooms_occupied = 1;
	$high_rooms_occupied = $low_rooms_occupied + 4;
	$low_occupancy_percent = $low_rooms_occupied / $rooms_avail * 100;
	$high_occupancy_percent = $high_rooms_occupied / $rooms_avail * 100;
	$am_room_attendants = ((($low_rooms_occupied + $high_rooms_occupied)/2)/$am_gra)* $GRA;
	$pm_room_attendants = ((($low_rooms_occupied + $high_rooms_occupied)/2)/$pm_gra)* $GRA;
	$am_housemen = 15;
	$pm_housemen = 15;
	$am_lobby_attendant = 7.5;
	$pm_lobby_attendant = 6;
	$public_area_attendants = 11.42;
	$laundry_attendants = ((($low_rooms_occupied + $high_rooms_occupied)/2)/$laundry_attendant)* $GRA_TWO;
	$room_coordinator = 8;
	$pm_floor_supervisor = 13.70;
	$director_two_managers = 11.43;

	$total_hours_per_day = $am_room_attendants + $pm_room_attendants + $am_housemen + $pm_housemen + $am_lobby_attendant + $pm_lobby_attendant + $public_area_attendants + $laundry_attendants + $room_coordinator + $pm_floor_supervisor + $director_two_managers;

	$total_dollars_per_day = (($am_room_attendants * 13.26) + ($pm_room_attendants * 13.26) + ($am_housemen * 13.26) + ($pm_housemen * 13.26) + ($am_lobby_attendant * 13.26) + ($pm_lobby_attendant * 13.26) + ($public_area_attendants * 13.26) + ($laundry_attendants * 13.26) + ($room_coordinator * 15) + ($pm_floor_supervisor * 14) + ($director_two_managers * 23.55));
	
	$sql = "INSERT INTO `MLB_Labor_Stds_2013`(`Rooms_Occupied_Low`, `Rooms_Occupied_High`, `Occupancy_Percent_Low`, `Occupancy_Percent_High`, `AM_Room_Attendants`, `PM_Room_Attendants`, `AM_Housemen`, `PM_Housemen`, `AM_Lobby_Attendant`, `PM_Lobby_Attendant`, `Public_Area_Attendant`, `Laundry_Attendant`, `AM_Room_Coordinator`, `PM_Floor_Supervisor`, `Director_And_Two_Managers`, `Total_Hours_Per_Day`, `Total_Dollars_Per_Day`) 
	VALUES($low_rooms_occupied, $high_rooms_occupied, $low_occupancy_percent, $high_occupancy_percent, $am_room_attendants, $pm_room_attendants, $am_housemen, $pm_housemen, $am_lobby_attendant, $pm_lobby_attendant, $public_area_attendants, $laundry_attendants, $room_coordinator, $pm_floor_supervisor, $director_two_managers, $total_hours_per_day, $total_dollars_per_day)";
	
	$result = mysql_query($sql) or die(mysql_error());
	$pm_lobby_attendant = 7.50;
	
	while($low_rooms_occupied + 5 <= $rooms_avail)
	{
		$low_rooms_occupied = $low_rooms_occupied + 5;
		$high_rooms_occupied = $low_rooms_occupied + 4;
		$low_occupancy_percent = $low_rooms_occupied / $rooms_avail * 100;
		$high_occupancy_percent = $high_rooms_occupied / $rooms_avail * 100;
		$am_room_attendants = ((($low_rooms_occupied + $high_rooms_occupied)/2)/$am_gra)* $GRA;
		$pm_room_attendants = ((($low_rooms_occupied + $high_rooms_occupied)/2)/$pm_gra)* $GRA;
		if($low_rooms_occupied >= 141)
		{
			$laundry_attendants = 30.0;
			if($low_rooms_occupied >= 181)
			{
				$am_housemen = 22.5;
			}
		}
		else
		{
			$laundry_attendants = ((($low_rooms_occupied + $high_rooms_occupied)/2)/$laundry_attendant)* $GRA_TWO;
		}
		$pm_lobby_attendant = 7.5;

		$total_hours_per_day = $am_room_attendants + $pm_room_attendants + $am_housemen + $pm_housemen + $am_lobby_attendant + $pm_lobby_attendant + $public_area_attendants + $laundry_attendants + $room_coordinator + $pm_floor_supervisor + $director_two_managers;

		$total_dollars_per_day = (($am_room_attendants * 13.26) + ($pm_room_attendants * 13.26) + ($am_housemen * 13.26) + ($pm_housemen * 13.26) + ($am_lobby_attendant * 13.26) + ($pm_lobby_attendant * 13.26) + ($public_area_attendants * 13.26) + ($laundry_attendants * 13.26) + ($room_coordinator * 15) + ($pm_floor_supervisor * 14) + ($director_two_managers * 23.55));

		$sql = "INSERT INTO `MLB_Labor_Stds_2013`(`Rooms_Occupied_Low`, `Rooms_Occupied_High`, `Occupancy_Percent_Low`, `Occupancy_Percent_High`, `AM_Room_Attendants`, `PM_Room_Attendants`, `AM_Housemen`, `PM_Housemen`, `AM_Lobby_Attendant`, `PM_Lobby_Attendant`, `Public_Area_Attendant`, `Laundry_Attendant`, `AM_Room_Coordinator`, `PM_Floor_Supervisor`, `Director_And_Two_Managers`, `Total_Hours_Per_Day`, `Total_Dollars_Per_Day`) 
	VALUES($low_rooms_occupied, $high_rooms_occupied, $low_occupancy_percent, $high_occupancy_percent, $am_room_attendants, $pm_room_attendants, $am_housemen, $pm_housemen, $am_lobby_attendant, $pm_lobby_attendant, $public_area_attendants, $laundry_attendants, $room_coordinator, $pm_floor_supervisor, $director_two_managers, $total_hours_per_day, $total_dollars_per_day)";
	
		$result = mysql_query($sql) or die(mysql_error());
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