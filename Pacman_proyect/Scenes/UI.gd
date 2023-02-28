extends CanvasLayer

var score := 0
var final_score := 0
var level_active := true

func level_won(lives: int):
	$Game_Win_Text.visible = true
	show_final_score(lives)

func add_score(value: int):
	score += value
	$Score.text = str(score)

func draw_lives(lives: int):
	$Life1.visible = lives >= 1
	$Life2.visible = lives >= 2
	$Life3.visible = lives >= 3
	$Life4.visible = lives >= 4
	$Life5.visible = lives >= 5

func show_final_score(lives: int):
	final_score = score + (300 * lives)
	$Final_Score.text = str(final_score)
	$Final_Score.visible = true
	$Final_ScoreLabel.visible = true
	$Score.visible = false
	$ScoreLabel.visible = false

func game_over():
	final_score = score
	$Final_Score.text = str(final_score)
	$Final_Score.visible = true
	$Final_ScoreLabel.visible = true
	$Score.visible = false
	$ScoreLabel.visible = false
	$Game_Over_Text.visible = true
