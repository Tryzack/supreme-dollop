extends Node2D

const tile_size = 8

export(Resource) var pellet_prefab
export var vulnerable_time := 7.0

var pellets_left = 244
var current_ghost := 0
var lives := 3
var ghost_bonus = 200
var mortal = true
var game_active = true
var scared_ghosts = 0
var are_ghost_scared = false
var is_siren_playing = []
var siren_tier_changed = false

onready var resettimer = $"AiResetTimer"
onready var player : PacMan = $"PacMan"
onready var red : red_ghost = $Enemies/Red
onready var blue : blue_ghost = $Enemies/Blue
onready var pink : pink_ghost = $Enemies/Pink
onready var orange : orange_ghost = $Enemies/Orange
onready var nav_2D : Navigation2D = $Arena/Arena_Navigator
onready var ui = $UI

func gamestart():
	player.movement_enabled = true
	red.state = 1
	red.enable_movement()
	$ReviveTimer.start()

func level_won():
	siren_stop()
	$Scatter_Timer.stop()
	$Retreat_Timer.stop()
	$ReviveTimer.stop()
	$sounds/power_pellet.stop()
	$sounds/retreating.stop()
	$sounds/win.play()
	game_active = false
	orange.state = 0
	red.state = 0
	blue.state = 0
	pink.state = 0
	orange.call_deferred("free")
	red.call_deferred("free")
	blue.call_deferred("free")
	player.call_deferred("free")
	pink.call_deferred("free")
	$Pellets.call_deferred("free")
	$UI.level_won(lives)

func score_checker():
	if $UI.score == 2000:
		lives += 1
		$sounds/life_gained.play()
	elif $UI.score == 4000:
		lives += 1
		$sounds/life_gained.play()
	elif $UI.score == 6000:
		lives += 1
		$sounds/life_gained.play()
	elif $UI.score == 8000:
		lives += 1
		$sounds/life_gained.play()
	$UI.draw_lives(lives)

func reset():
	orange.enable_movement()
	orange.reset()
	pink.enable_movement()
	pink.reset()
	blue.enable_movement()
	blue.reset()
	red.enable_movement()
	red.reset()

func _on_ExitR_body_entered(body):
	if body is PacMan:
		body.warp_to($"Arena/Exit-L".global_position)

func _on_ExitL_body_entered(body):
	if body is PacMan:
		body.warp_to($"Arena/Exit-R".global_position)

func _on_Pellet_pellet_eaten():
	pellets_left -= 1
	if pellets_left == 199 or pellets_left == 159 or pellets_left == 119 or pellets_left == 199 or pellets_left == 79:
		siren_tier_changed = true
	if siren_tier_changed == true and are_ghost_scared == false:
		siren_play()
		siren_tier_changed = false
		
	ui.add_score(10)
	if pellets_left == 0:
		level_won()
	score_checker()
	if pellets_left %2 == 1:
		$sounds/munch1.play()
	else:
		$sounds/munch2.play()

func _on_Power_Pellet_pellet_eaten():
	$Scatter_Timer.stop()
	$Retreat_Timer.stop()
	$sounds/retreating.stop()
	siren_stop()
	$sounds/power_pellet.play()
	$Scatter_Timer.start()
	ui.add_score(50)
	pellets_left -= 1
	if pellets_left == 0:
		level_won()
	if pink.state == 1:
		scared_ghosts += 1
		pink.scared()
	elif pink.state == 2:
		pink.scared()
	if red.state == 1:
		scared_ghosts += 1
		red.scared()
	elif red.state == 2:
		red.scared()
	if blue.state == 1:
		scared_ghosts += 1
		blue.scared()
	elif blue.state == 2:
		blue.scared()
	if orange.state == 1:
		scared_ghosts += 1
		orange.scared()
	elif orange.state == 2:
		orange.scared()
	if scared_ghosts > 0:
		are_ghost_scared = true
	score_checker()

func _on_Scatter_Timer_timeout():
	$sounds/power_pellet.stop()
	$sounds/retreating.play()
	$Retreat_Timer.start()

func _on_PacMan_player_reset():
	if lives >= 0:
		player.movement_enabled = true
	else:
		orange.call_deferred("free")
		red.call_deferred("free")
		blue.call_deferred("free")
		player.call_deferred("free")
		pink.call_deferred("free")
		$Pellets.call_deferred("free")
		$UI.game_over()

func _on_ghost_ate_pacman():
	orange.state = 4
	red.state = 4
	blue.state = 4
	pink.state = 4
	player.movement_enabled = false
	player.die()
	siren_stop()
	$sounds/death1.play()
	$ReviveTimer.stop()
	$sounds/power_pellet.stop()
	$sounds/retreating.stop()
	$Scatter_Timer.stop()
	$Retreat_Timer.stop()
	lives = lives-1
	$UI.draw_lives(lives)
	resettimer.start()

func _on_AiResetTimer_timeout():
	if lives >= 0:
		reset()

func giveRedPos():
	return red.global_position

func _on_ReviveTimer_timeout():
	if lives >= 0 and game_active == true:
		if red.state == 4 and red != null:
			red.enable_movement()
			red.state = 1
			red.speed_multiplier = 1
		elif pink.state == 4 and pink != null:
			pink.state = 1
			pink.enable_movement()
			pink.speed_multiplier = 1
		elif blue.state == 4 and blue != null:
			blue.state = 1
			blue.enable_movement()
			blue.speed_multiplier = 1
		elif orange.state == 4 and orange != null:
			orange.state = 1
			orange.enable_movement()
			orange.speed_multiplier = 1

func _on_pacman_ate_ghost():
	scared_ghosts -= 1
	if scared_ghosts == 0:
		$Scatter_Timer.stop()
		$Retreat_Timer.stop()
		$sounds/retreating.stop()
		$sounds/power_pellet.stop()
		are_ghost_scared = false
		siren_play()
	$sounds/eat_ghost.play()
	$UI.add_score(100)
	score_checker()

func _on_INTRO_finished():
	gamestart()
	siren_play()
	$UI/Get_ready.visible = false

func _on_death1_finished():
	$sounds/death2.play()

func _on_death2_finished():
	$sounds/death3.play()

func _on_death3_finished():
	if lives >= 0:
		player.movement_enabled = true
		$ReviveTimer.start()
		red.state = 1
		red.enable_movement()
		siren_play()

func siren_play():
	if pellets_left >= 200:
		$sounds/siren1.play()
	elif pellets_left >= 160:
		$sounds/siren1.stop()
		$sounds/siren2.play()
	elif pellets_left >= 120:
		$sounds/siren2.stop()
		$sounds/siren3.play()
	elif pellets_left >= 80:
		$sounds/siren3.stop()
		$sounds/siren4.play()
	else:
		$sounds/siren4.stop()
		$sounds/siren5.play()
		
func siren_stop():
	$sounds/siren1.stop()
	$sounds/siren2.stop()
	$sounds/siren3.stop()
	$sounds/siren4.stop()
	$sounds/siren5.stop()

func _on_Retreat_Timer_timeout():
	$sounds/retreating.stop()
	siren_play()
	scared_ghosts = 0
	are_ghost_scared = false
