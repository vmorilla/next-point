	SECTION PAGE_38

	PUBLIC _sprite_ball_shadow, _sprite_ball, _sprite_net, _sprite_player_back_a_00, _sprite_player_back_a_01, _sprite_umpire_head_00, _sprite_umpire_head_01, _sprite_umpire_head_02, _sprite_umpire_head_03

_sprite_ball_shadow:
	db 0x01, 0xf8, 0xf0
	incbin "../assets/sprite_ball_shadow.bin"

_sprite_ball:
	db 0x01, 0xf8, 0xf0
	incbin "../assets/sprite_ball.bin"

_sprite_net:
	db 0x0d, 0xe0, 0xe0
	incbin "../assets/sprite_net.bin"

_sprite_player_back_a_00:
	db 0x06, 0xf6, 0xd4
	incbin "../assets/sprite_player_back_a_00.bin"

_sprite_player_back_a_01:
	db 0x06, 0xf6, 0xdb
	incbin "../assets/sprite_player_back_a_01.bin"

_sprite_umpire_head_00:
	db 0x01, 0xfb, 0xf2
	incbin "../assets/sprite_umpire_head_00.bin"

_sprite_umpire_head_01:
	db 0x01, 0xfe, 0xf2
	incbin "../assets/sprite_umpire_head_01.bin"

_sprite_umpire_head_02:
	db 0x01, 0xfc, 0xf2
	incbin "../assets/sprite_umpire_head_02.bin"

_sprite_umpire_head_03:
	db 0x01, 0xfd, 0xf1
	incbin "../assets/sprite_umpire_head_03.bin"

