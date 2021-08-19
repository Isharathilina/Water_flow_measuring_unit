<?php
$servername = "localhost";
$username = "root";
$password = "root";
$dbname = "test";

$d1 = $_POST["data1"];
$d2 = $_POST["data2"];


// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
  die("Connection failed: " . $conn->connect_error);
}

$sql = "INSERT INTO student (nameID, addr, phone) VALUES (19, '$d1', '$d2')";

if ($conn->query($sql) === TRUE) {
  echo "New record created successfully";
} else {
  echo "Error: " . $sql . "<br>" . $conn->error;
}

$conn->close();
?>