<?php
$servername = "localhost";
$username = "root";
$password = "";
$dbname = "sensor_data";

$con = mysqli_connect($servername,$username,$password,$dbname); 

echo $Light_Intensity=$_GET['Light_Intensity']. "<br/>";
echo $Distance=$_GET['Distance']. "<br/>";

 
$sql = "INSERT INTO lab6_data (MCU_ID,Light_Intensity,Distance) VALUES ((SELECT MCU_ID FROM mcu WHERE Name ='ESP2'),'{$Light_Intensity}','{$Distance}')";

if (mysqli_query($con, $sql)) {
echo "New record created successfully";
} else {
echo "Unsuccessful";
}


?>
