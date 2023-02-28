extends KinematicBody2D
class_name PacMan

signal player_reset

export var speed := 100
export var current_dir = "none"
export var movement_direction := Vector2.ZERO
var movement_enabled = false
var velocity := Vector2()
var inputs = {"ui_right": Vector2.RIGHT,
			"ui_left": Vector2.LEFT,
			"ui_up": Vector2.UP,
			"ui_down": Vector2.DOWN}

func get_input():
	if movement_enabled:
		velocity = Vector2()
		for input in inputs:
			if Input.is_action_just_pressed(input):
				current_dir = input
				movement_direction = inputs[input]
				animate_movement()
				
		velocity = movement_direction * speed

func animate_movement():
	$AnimatedSprite.play("movimiento")
	match current_dir:
		"ui_right":
			$AnimatedSprite.rotation_degrees = 0
			$AnimatedSprite.flip_h = false
		"ui_left":
			$AnimatedSprite.rotation_degrees = 0
			$AnimatedSprite.flip_h = true
		"ui_up":
			$AnimatedSprite.rotation_degrees = -90
			$AnimatedSprite.flip_h = false
		"ui_down":
			$AnimatedSprite.rotation_degrees = 90
			$AnimatedSprite.flip_h = false

func die():
	$AnimatedSprite.play("Dead")
	var _d = $AnimatedSprite.connect("animation_finished", self, "reset")

func _physics_process(_delta):
	if !movement_enabled:
		return
	
	get_input()
	velocity = move_and_slide(velocity)
	if velocity.length() < 1 and movement_direction != Vector2.ZERO:
		movement_direction = Vector2.ZERO
		current_dir = "none"
		$AnimatedSprite.playing = false


func warp_to(pos):
		global_position = pos

func reset():
	if $AnimatedSprite.is_connected("animation_finished", self, "reset"):
		$AnimatedSprite.disconnect("animation_finished", self, "reset")
	position = Vector2(222, 424)
	$AnimatedSprite.play("Idle")
	current_dir = "none"
	movement_direction = Vector2.ZERO
	emit_signal("player_reset")
	movement_enabled = false
