<?php
$servername = "localhost";
$username = "root";
$password = "";
$dbname = "sensor_data";

$con = mysqli_connect($servername,$username,$password,$dbname); 

echo $Temperature=$_GET['Temperature']. "<br/>";
echo $Humidity=$_GET['Humidity']. "<br/>";

 
$sql = "INSERT INTO lab6_data (MCU_ID,Temperature,Humidity) VALUES ((SELECT MCU_ID FROM mcu WHERE Name ='ESP2'),'{$Temperature}','{$Humidity}')";

if (mysqli_query($con, $sql)) {
echo "New record created successfully";
} else {
echo "Unsuccessful";
}


?>
