<?php
header('Refresh: 0.5');
$con  = mysqli_connect("localhost","root","root","test");
 if (!$con) {
     # code...
    echo "Problem in database connection! Contact administrator!" . mysqli_error();
 }else{
         $sql ="SELECT * FROM waterflowdata ORDER BY id DESC LIMIT 10";
         $result = mysqli_query($con,$sql);
         $chart_data="";
         while ($row = mysqli_fetch_array($result)) { 
 
            $timeSet[]  = $row['time']  ;
            $flowRates[] = $row['rate'];
			
        }
 
 
	$sql = "SELECT rate, total, time FROM waterflowdata ORDER BY id DESC LIMIT 1";  // ORDER BY column_name DESC LIMIT 1;
	$result = $con->query($sql);

	if ($result->num_rows > 0) {
	  echo "<table border='2' class='timecard'><caption>Flow Rate Data</caption><tr class='odd'><th>Flow Rate</th><th>Total</th><th>Time</th></tr>";
	  // output data of each row
	  while($row = $result->fetch_assoc()) {
		echo "<tr class='even'> <td>".$row["rate"]." LPM"."</td> <td>".$row["total"]." Liters"."</td> <td>".$row["time"]."</td> </tr>";  // <td>".$row["total"]." ".$row["time"]."</td>
	  }
	  echo "</table>";
	} else {
	  echo "0 results";
	}
}
 
$con->close();
?>
<!DOCTYPE html>
<html lang="en"> 
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Graph</title> 
		
		<style type="text/css">
		
		#main {
			width: 780px;
			padding: 20px;
			margin: auto;
		}

		table.timecard {
			margin: auto;
			width: 600px;
			border-collapse: collapse;
			border: 1px solid #fff; /*for older IE*/
			border-style: hidden;
		}

		table.timecard caption {
			background-color: #f79646;
			color: #fff;
			font-size: large;
			font-weight: bold;
			letter-spacing: .3em;
		}

		table.timecard thead th {
			padding: 8px;
			background-color: #fde9d9;
			font-size: large;
		}

		table.timecard thead th#thDay {
			width: 40%;	
		}

		table.timecard thead th#thRegular, table.timecard thead th#thOvertime, table.timecard thead th#thTotal {
			width: 20%;
		}

		table.timecard th, table.timecard td {
			padding: 3px;
			border-width: 1px;
			border-style: solid;
			border-color: #f79646 #ccc;
		}

		table.timecard td {
			text-align: right;
		}

		table.timecard tbody th {
			text-align: left;
			font-weight: normal;
		}

		table.timecard tfoot {
			font-weight: bold;
			font-size: large;
			background-color: #687886;
			color: #fff;
		}

		table.timecard tr.even {
			background-color: #fde9d9;
		}
		</style>
		
    </head>
    <body>
        <div style="width:60%;hieght:20%;text-align:center">
            <h2 class="page-header" >Live Analytics </h2>
            <div>Water Flow Rate According to Time</div>
            <canvas  id="chartjs_bar"></canvas> 
        </div>    
    </body>
  <script src="//code.jquery.com/jquery-1.9.1.js"></script>
  <script src="//cdnjs.cloudflare.com/ajax/libs/Chart.js/2.4.0/Chart.min.js"></script>
<script type="text/javascript">
      var ctx = document.getElementById("chartjs_bar").getContext('2d');
                var myChart = new Chart(ctx, {
                    type: 'bar',
                    data: {
                        labels:<?php echo json_encode($timeSet); ?>,
                        datasets: [{
                            backgroundColor: [
                               "#5969ff",
                                "#ff407b",
                                "#25d5f2",
                                "#ffc750",
                                "#2ec551",
                                "#7040fa",
                                "#ff004e"
                            ],
                            data:<?php echo json_encode($flowRates); ?>,
                        }]
                    },
                    options: {
                           legend: {
                        display: true,
                        position: 'bottom',
 
                        labels: {
                            fontColor: '#71748d',
                            fontFamily: 'Circular Std Book',
                            fontSize: 14,
                        }
                    },
 
 
                }
                });
    </script>
</html>