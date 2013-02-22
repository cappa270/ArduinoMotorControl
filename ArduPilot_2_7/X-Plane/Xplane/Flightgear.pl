#!/usr/bin/perl -w

# Heavily modified and adapted by Jason Short And Michael Oborne
# from http://www.perl.com - Autopilots in Perl
# By Jeffrey Goff on July 12, 2004

use strict;
use IO::Socket;
use IO::Select;
$| = 1;

my $rh;
my $x_plane_ip		= '127.0.0.1';
my $ser_port	 	= 5331;
my $receive_port	= 49005;
my $transmit_port	= 49000;
my $throttle_test = 0;
my $check_a = 0;
my $check_b = 0;
my $count = 0;

my $roll_out = 0;
my $pitch_out = 0;
my $throttle_out = 0;
my $rudder_out = 0;
my $wp_distance = 0;
my $wp_index = 0;
my $control_mode = 0;
my $control_mode_alpha;
my $bearing_error = 0;
my $alt_error = 0;
my $energy_error = 0;

my $extraInt = 0; # for debugging
my $int_1 = 0;
my $int_2 = 0;
my $int_3 = 0;
my $int_4 = 0;

my $diyd_header = "DIYd";

my $Xplane_in_sock = IO::Socket::INET->new(
	LocalPort => $receive_port,
	LocalAddr => $x_plane_ip,
	Proto		 => 'udp')
	or die print "error creating receive port for $x_plane_ip: $@\n";

my $receive = IO::Select->new();


my $Xplane_out_sock = IO::Socket::INET->new(
	PeerPort	=> $transmit_port,
	PeerAddr	=> $x_plane_ip,
	Proto		 => 'tcp')
	or die print "error creating transmit port for $x_plane_ip: $@\n";

my $transmit = IO::Select->new();



#open sockets on 7070
my $arduSocket = IO::Socket::INET->new(	Proto => 'tcp',
											PeerAddr =>	'127.0.0.1',
										   	PeerPort => $ser_port
										   	) or die print "Can not connect to Serial $!";
										   	#,Type = 'SOCK_STREAM'
										   	
										   	
$receive->add($Xplane_in_sock);
$receive->add($arduSocket);
$transmit->add($Xplane_out_sock);
$arduSocket->autoflush(1);

sub parseXplane {
	my $message = shift;
	#37.572923,-122.364941,167.009546,-6.223989,-1.347284,93.487282,89.581045
	if (!($message =~ /^([^,]+),([^,]+),([^,]+),([^,]+),([^,]+),([^,]+),([^,]+)/))
	{ return; }
	
	#convert data	
	my $lat 		= int($1 * 10000000);
	my $lng 		= int($2 * 10000000);
	my $altitude 	= int($3 * 3.048);		# altitude to meters
	my $speed 		= int($7 * 044.704);		# spped to m/s * 100
	my $airspeed 	= int($7 * 044.704);		# speed to m/s * 100
	my $pitch 		= int($5 * 100);
	my $roll 		= int($4 * 100);
	my $heading 	= int($6 * 100);
	
	# format and publish the IMU style data to the Ardupilot
	my $outgoing = pack("CCs<4", 8,4, $roll, $pitch, $heading, $airspeed);
	$check_a = $check_b = 0;
	for( split(//,$outgoing) )
	{
		$check_a += ord;
		$check_a %= 256;
		$check_b += $check_a;
		$check_b %= 256;
		#print ord . "\n";
	}
	$outgoing = pack("a4CCs<4CC","DIYd", 8,4, $roll, $pitch, $heading, $airspeed, $check_a,$check_b);
	$arduSocket->send($outgoing);

	$count++;
	if ($count == 10){
		# count of 10 = 5 hz, 50 = 1hz
		$count = 0;
		$outgoing = pack("CCl<2s<3", 14,3, $lng, $lat, $altitude, $speed, $heading);
		$check_a = $check_b = 0;
		for( split(//,$outgoing) )
		{
			$check_a += ord;
			$check_a %= 256;
			$check_b += $check_a;
			$check_b %= 256;
			#print ord . "\n";
		}
		$outgoing = pack("a4CCl<2s<3CC","DIYd", 14,3, $lng, $lat, $altitude, $speed, $heading, $check_a, $check_b);
		$arduSocket->send($outgoing);
	}
	#print "lat = $lat, long = $lng, alt = $altitude\n";
	
	$count = 0 if ($count > 50);
}

sub parseMessage
{
	my ($data) = @_;
	my @out = unpack("s8C2",$data);
	
	#$,="\t";
	#print @out;
	
	$roll_out = $out[0] *1 / 4500  if (defined $out[0]);
	$roll_out = 1 if($roll_out > 1);
	$roll_out = -1 if($roll_out < -1);
	
	$pitch_out = $out[1] * -1 / 4500  if (defined $out[1]);
	$pitch_out = 1 if($pitch_out > 1);
	$pitch_out = -1 if($pitch_out < -1);


	$throttle_out = $out[2] / 100  if (defined $out[2]);
	

	$rudder_out = $out[3] * 1 / 4500  if (defined $out[3]);
	$rudder_out = 1 if($rudder_out > 1);
	$rudder_out = -1 if($rudder_out < -1);

	# normal reading
	$wp_distance	= $out[4] if (defined $out[4]);
	$bearing_error 	= $out[5] / 100  if (defined $out[5]);
	$alt_error 		= $out[6] if (defined $out[6]);
	$energy_error 	= $out[7] if (defined $out[7]);
	$wp_index 		= $out[8] if (defined $out[8]);
	$control_mode 	= $out[9] if (defined $out[9]);

	#debugging - send any three ints for debugging!
	$int_1	= $out[4] * 1  if (defined $out[4]);
	$int_2 	= $out[5] * 1  if (defined $out[5]);
	$int_3 	= $out[6] * 1  if (defined $out[6]);
	$int_4 	= $out[7] * 1  if (defined $out[7]);

	SWITCH: {
	  $control_mode == 0 && do { 	$control_mode_alpha = "Manual"; last SWITCH; };
	  $control_mode == 1 && do { 	$control_mode_alpha = "CIRCLE"; last SWITCH; };
	  $control_mode == 2 && do { 	$control_mode_alpha = "STABILIZE"; last SWITCH; };
	  $control_mode == 5 && do { 	$control_mode_alpha = "FLY_BY_WIRE_A"; last SWITCH; };
	  $control_mode == 6 && do { 	$control_mode_alpha = "FLY_BY_WIRE_B"; last SWITCH; };
	  $control_mode == 10 && do { 	$control_mode_alpha = "AUTO"; last SWITCH; };
	  $control_mode == 11 && do { 	$control_mode_alpha = "RTL"; last SWITCH; };
	  $control_mode == 12 && do { 	$control_mode_alpha = "LOITER"; last SWITCH; };
	  $control_mode == 13 && do { 	$control_mode_alpha = "TAKEOFF"; last SWITCH; };
	  $control_mode == 14 && do { 	$control_mode_alpha = "LAND"; last SWITCH; };
	}

	$bearing_error = sprintf("%.1f", $bearing_error);
	if ($count){
		printf("roll: %.3f,  pitch: %.3f,   thr: %.3f, rud: %.3f \r",$roll_out,$pitch_out,$throttle_out,$rudder_out);
		#print "roll: $roll_out,  pitch: $pitch_out,   thr: $throttle_out \n";
		#print "wp_dist:$wp_distance \tbearing_err:$bearing_error \talt_error:$alt_error \tenergy_err:$energy_error \tWP:$wp_index \tmode:$control_mode_alpha\n";
		#print "next_WP.alt:$int_1 \toffset_altitude:$int_2 \talt_error:$alt_error \ttarget_altitude:$int_4 \tWP:$wp_index \tmode:$control_mode_alpha\n";
		#print "test_alt:$int_1 \ttarget_altitude:$int_2 \tnext_wp.alt:$int_3 \toffset_altitude:$int_4 \tWP:$wp_index \tmode:$control_mode_alpha\n";
		#print "throttle_cruise: $int_1, landing_pitch: $int_2, throttle: $throttle_out, altitude_error: $int_3,  WP: $wp_index,   mode: $control_mode\n";
	}
	if ($count % 5 == 0) {
		print $Xplane_out_sock "3,$roll_out,$pitch_out,$rudder_out,$throttle_out\r\n";
	}
}

sub readline {
	my ($rh) = @_;
	my $temp = "";
	my $step = 0;
	my $message = "";
	
	while (1) {
		$rh->recv($message,1);
		$temp .= $message;
		
		if ($temp =~ /^AAA/) {
			$step++;
			if ($temp =~ /\n/ && $step >= 21) { last; } # data
		}
		if ($temp =~ /\n/ && $step == 0) { # normal message
			last;
		}
	}
	return $temp;
}

sub main_loop {
	my ($receive,$transmit) = @_;
	my $recv_data = "";

	while(1) {
		my ($rh_set) = IO::Select->select($receive, undef, undef, .1);


		foreach $rh (@$rh_set) {
			if ($rh == $Xplane_in_sock) {
				my $message;
				$rh->recv($message,1024);
				parseXplane($message);
				
			}elsif ($rh == $arduSocket) {
				my $message = '';
				
				$message = &readline($rh);
				
				if($message =~ '^AAA'){
					$message = substr $message, 3, 20; 
					parseMessage($message);
				}elsif($message =~ '^XXX'){
					print ": " . time. " $message";
				}else{
					print (" " x 50);
					print "\r: $message";
				}
				$rh->flush();
				#print "$message \n";
			}
		}

	}
}

main_loop($receive, $transmit);
