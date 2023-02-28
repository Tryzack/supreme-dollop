extends KinematicBody2D
class_name pink_ghost

const tile_size = 8

signal pacman_ate_ghost
signal ghost_ate_pacman

export(int) var speed: int = 95
export(float) var speed_multiplier: float = 1
var velocity: Vector2 = Vector2.ZERO
var enable_movement = false

var path: Array = []
var levelNavigation: Navigation2D = null
var player = null
var red = null
var starting_pos
var respawn_pos = Vector2(224, 282)
var state = 4
var new_pos = Vector2(rand_range(25, 424), rand_range(72, 520))
onready var restore_timer = $RestoreTimer
onready var scatter_timer = $Scatter_Timer
onready var ai_timer = $AI_Timer
onready var redline = $Line2D

func _ready():
	starting_pos = global_position
	yield(get_tree(), "idle_frame")
	var tree = get_tree()
	if tree.has_group("LevelNavigation"):
		levelNavigation = tree.get_nodes_in_group("LevelNavigation")[0]
	if tree.has_group("Player"):
		player = tree.get_nodes_in_group("Player")[0]
	if tree.has_group("Respawn"):
		respawn_pos = tree.get_nodes_in_group("Respawn")[0]
	if tree.has_group("red"):
		red = tree.get_nodes_in_group("red")[0]
		
func scared():
	state = 2
	scatter_timer.start()
	
func _physics_process(_delta):
	if enable_movement == true:
		if state == 1:
			redline.global_position = Vector2.ZERO
			if player and levelNavigation:
				generate_path()
				navigate()
			move()
			
		elif state == 2:
			redline.global_position = Vector2.ZERO
			if player and levelNavigation:
				generate_path()
				navigate()
			move()
			print("Scared")
			
		elif state == 3:
			redline.global_position = Vector2.ZERO
			if player and levelNavigation:
				generate_path()
				navigate()
			move()
			print("Dead")
		elif state == 4:
			$AnimatedSprite.play("Idle")
		animate()
	
func navigate():
	if path.size() > 0:
		velocity = global_position.direction_to(path[1])*speed*speed_multiplier

func generate_path():
	
	if state == 1:
		if levelNavigation != null and player != null:
			var distance = global_position.distance_to(player.position)
			if distance < 5 * tile_size:
				path = levelNavigation.get_simple_path(global_position, player.position, false)
			else:
				var target_position = player.position + (player.velocity.normalized() * 4 * tile_size)
				path = levelNavigation.get_simple_path(global_position, target_position, false)
			redline.points = path
			
	elif state == 2:
		path = levelNavigation.get_simple_path(global_position, new_pos, false)
		redline.points = path
		speed_multiplier = 0.5
		
	elif state == 3:
		speed_multiplier = 1.5
		path = levelNavigation.get_simple_path(global_position, respawn_pos, false)

func create_random_path():
	new_pos = Vector2(rand_range(25, 424), rand_range(72, 520))

func move():
	velocity = move_and_slide(velocity)

func _on_Area_body_entered(_body):
	if state == 1:
		emit_signal("ghost_ate_pacman")
	elif state == 2:
		emit_signal("pacman_ate_ghost")
		state = 3
		dead()

func dead():
	restore_timer.start()

func reset():
	$RestoreTimer.stop()
	$Scatter_Timer.stop()
	state = 4
	global_position = starting_pos

func _on_Scatter_Timer_timeout():
	if state == 2:
		state=1
		speed_multiplier = 1


func _on_RestoreTimer_timeout():
	get_node("CollisionShape2D").disabled = false
	get_node("Area/CollisionShape2D").disabled = false
	$AnimatedSprite.play("Idle")
	state = 4
	speed_multiplier = 1
	
func animate():
	match state:
		1:
			if abs(velocity.x) > abs(velocity.y):
				if velocity.x > 0:
					$AnimatedSprite.play("Moving_Right")
				else:
					$AnimatedSprite.play("Moving_Left")
			else:
				if velocity.y > 0:
					$AnimatedSprite.play("Moving_Down")
				else:
					$AnimatedSprite.play("Moving_Up")
		2:
			$AnimatedSprite.play("Scared")
		3:
			if abs(velocity.x) > abs(velocity.y):
				if velocity.x > 0:
					$AnimatedSprite.play("Dead_Right")
				else:
					$AnimatedSprite.play("Dead_Left")
			else:
				if velocity.y > 0:
					$AnimatedSprite.play("Dead_Down")
				else:
					$AnimatedSprite.play("Dead_Up")

# warning-ignore:function_conflicts_variable
func enable_movement():
	enable_movement = true

func disable_movement():
	enable_movement = false
