<?php
session_start();

//variables
$name = $_POST['user'];

//flags
$session_flag = true;
$timeout_flag = false;

$conn_flag = true;
$query_flag = true;
$user_flag = true;
$delete_flag = true;

//check valid
if( empty($_SESSION['userid']) ) {
	$session_flag = false;
}
elseif( $_SESSION['time'] < time() - 3600 ) {
	//timeout
	$timeout_flag = true;
	$_SESSION['userid'] = '';
}
else {
	$_SESSION['time'] = time();  //update

	//input
	if( empty($name) ) {
		$user_flag = false;
	}
	elseif( $name == '$$ADMIN-NAME$$' ) {
		$delete_flag = false;
	}
	else {
		//database
		$conn_string = "host=$$localhost$$ port=5432 dbname=$$DB-NAME$$ user=$$DB-USER$$ password=$$DB-PWD$$";
		$dbconn = pg_connect($conn_string);
		if( $dbconn ) {
			//query
			$query_string = "SELECT * FROM $$TABLE-NAME$$ WHERE $$FIELD-USER$$='$name';";
			$result = pg_query($dbconn, $query_string);
			if( $result ) {
				$rows = pg_num_rows($result);
				if( $rows == 0 ) {
					$user_flag = false;
				}
				elseif( $rows > 1 ) {
					$delete_flag = false;
				}
				else {
					//delete
					$query_string = "DELETE FROM $$TABLE-NAME$$ WHERE $$FIELD-USER$$='$name';";
					$result = pg_query($dbconn, $query_string);
					if( !$result ) {
						$delete_flag = false;
					}
				}
			}
			else {
				$query_flag = false;
			}
			//close
			pg_close($dbconn);
		}
		else {
			$conn_flag = false;
		}
	}
}
?>

<!DOCTYPE html>
<html>

<head>
<meta charset="UTF-8">
<title>
<?php if( !$session_flag ) : ?>
$$SESSION_INVALID_TITLE$$
<?php elseif( $timeout_flag ) : ?>
$$TIMEOUT_TITLE$$
<?php elseif( $conn_flag == false ) : ?>
$$CONNECT_FAILED_TITLE$$
<?php elseif( $query_flag == false ) : ?>
$$QUERY_FAILED_TITLE$$
<?php elseif( !$user_flag ) : ?>
$$ERROR_USER_TITLE$$
<?php elseif( !$delete_flag ) : ?>
$$DELETE_ERROR_TITLE$$
<?php else : ?>
$$TITLE$$
<?php endif; ?>
</title>
</head>

<body>

<?php if( !$session_flag ) : ?>
<h1>The current session is invalid!</h1>
<p><a href="index.htm">Return to log in page!</a></p>
<?php elseif( $timeout_flag ) : ?>
<h1>The current session expired!</h1>
<p><a href="index.htm">Return to log in page!</a></p>
<?php elseif( $conn_flag == false ) : ?>
<h1>Cannot connect the database!</h1>
<p><a href="admin.php?<?php echo htmlspecialchars(SID); ?>">Return to administrative page!</a></p>
<?php elseif( $query_flag == false ) : ?>
<h1>Query failed!</h1>
<p><a href="admin.php?<?php echo htmlspecialchars(SID); ?>">Return to administrative page!</a></p>
<?php elseif( !$user_flag ) : ?>
<h1>User does not exist!</h1>
<p><a href="admin.php?<?php echo htmlspecialchars(SID); ?>">Return to administrative page!</a></p>
<?php elseif( !$delete_flag ) : ?>
<h1>Delete failed!</h1>
<p><a href="admin.php?<?php echo htmlspecialchars(SID); ?>">Return to administrative page!</a></p>
<?php else : ?>
<h1>Deletion is successful!</h1>
<p><a href="admin.php?<?php echo htmlspecialchars(SID); ?>">Return to administrative page!</a></p>
<?php endif; ?>

</body>

</html>
