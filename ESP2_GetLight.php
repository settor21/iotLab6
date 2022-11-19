<?php
    $servername = "localhost";
    $username = "root";
    $password = "";
    $dbname = "sensor_data";

    $con = mysqli_connect($servername,$username,$password,$dbname);
    
    $data=array(); 

   $sql = "SELECT * FROM (
    SELECT * 
    FROM lab6_data 
    WHERE (MCU_ID = 2)  ORDER BY ID DESC LIMIT 5
  ) AS `table` ORDER by ID ASC";

   if ($q=mysqli_query($con, $sql)) {
    while ($row = $q->fetch_assoc()){
        echo "{$row['Light_Intensity']}  {$row['Time']} \n\n\n"; //."<br/>";
    }
   } 
   
?>