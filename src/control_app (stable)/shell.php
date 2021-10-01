<?php
if(isset($_POST["launch"]))
{
	echo "Hello";
	shell_exec("bash '/home/spiderbotgold/SeniorDesignSpiderBot/catkin_ws/src/robot_gui_bridge/gui/startup.sh'");
}
?>
