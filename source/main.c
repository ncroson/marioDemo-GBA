#include <string.h>

#include <stdio.h>
#include <tonc.h>
#include <maxmod.h>
#include "soundbank.h"
#include "soundbank_bin.h"
#include "marioMap.h"
#include "mario.h"


OBJ_ATTR obj_buffer[128];
OBJ_AFFINE *obj_aff_buffer= (OBJ_AFFINE*)obj_buffer;

OBJ_ATTR *mario= &obj_buffer[0];



int gravity = 1;
int jumpSpeed = -12;

int mx= 96, my= 112;
int backx = 0, backy =80;
bool facingRight = true;
bool isJumping = false;
int frame = 0;

u32 tid= 0, pb= 0; //tile id, pal-bank

void init()
{
	//load palette
	memcpy(pal_bg_mem, marioMapPal, marioMapPalLen);
	//load tiles into CBB 0
	memcpy(&tile_mem[0][0], marioMapTiles, marioMapTilesLen);
	//load map into SBB 30
	memcpy(&se_mem[30][0], marioMapMap, marioMapMapLen);
	
	//place the tiles of 4bpp mario sprite
	//into LOW obj memory (cbb == 4)
	memcpy(&tile_mem[4][0], marioStandTiles, marioStandTilesLen);
	memcpy(pal_obj_mem, marioStandPal, marioStandPalLen);
	memcpy(&tile_mem[4][8], marioWalk1Tiles, marioWalk1TilesLen);
	memcpy(pal_obj_mem, marioWalk1Pal, marioWalk1PalLen);
	memcpy(&tile_mem[4][16], marioWalk2Tiles, marioWalk2TilesLen);
	memcpy(pal_obj_mem, marioWalk2Pal, marioWalk2PalLen);
	memcpy(&tile_mem[4][24], marioWalk3Tiles, marioWalk3TilesLen);
	memcpy(pal_obj_mem, marioWalk3Pal, marioWalk3PalLen);
	memcpy(&tile_mem[4][32], marioTurnTiles, marioTurnTilesLen);
	memcpy(pal_obj_mem, marioTurnPal, marioTurnPalLen);
	memcpy(&tile_mem[4][40], marioJumpTiles, marioJumpTilesLen);
	memcpy(pal_obj_mem, marioJumpPal, marioJumpPalLen);
	memcpy(&tile_mem[4][48], marioDuckTiles, marioDuckTilesLen);
	memcpy(pal_obj_mem, marioDuckPal, marioDuckPalLen);
	
	//initialize all sprites
	oam_init(obj_buffer, 128);
	
	//set up BG0 for 4bpp 64x32t map, uisng
	//  charblock 0 and screenblock 31
	REG_BG0CNT= BG_CBB(0) | BG_SBB(30) | BG_4BPP | BG_REG_64x32;
	REG_DISPCNT= DCNT_MODE0 | DCNT_BG0 | DCNT_OBJ | DCNT_OBJ_1D;
	
	obj_set_attr(mario,
		ATTR0_TALL,
		ATTR1_SIZE_16x32,
		ATTR2_PALBANK(pb) | tid);
	
	obj_set_pos(mario, mx, my);

}

void scroll_screen(){
	//scroll around
	if(mx > 140){
		mx = 140;
		REG_BG0HOFS= backx++;
	}
	else if(mx < 60){
		mx = 60;
		REG_BG0HOFS= backx--;
	}
	REG_BG0VOFS= backy;

}

void jump()
{
	my += jumpSpeed;
	jumpSpeed += gravity;
	tid = 40;
		
	if(jumpSpeed > 12){
		jumpSpeed = -12;
		isJumping = false;
		tid = 0;
	}
	
	
	/*
	y += vel;
			vel += acc;
				
			if(vel > 16){
				vel = -16;
				isJumping = false;
			}
	}
	*/
}

void move_mario()
{
	
	key_poll();
	
	if(key_hit(KEY_RIGHT) && facingRight == false){
		mario->attr1 ^= ATTR1_HFLIP;
		facingRight = true;
	}
	
	if(key_hit(KEY_LEFT) && facingRight == true){
		mario->attr1 ^= ATTR1_HFLIP;
		facingRight = false;
	}

	if(key_is_down(KEY_RIGHT)){
		
		mx++;
		
		if(!(frame & 7)){
			tid+= 8;
			if(tid > 24)
				tid = 8;
		}
	}
	
	else if(key_is_down(KEY_LEFT)){
		mx--;
		if(!(frame & 7)){
			tid+= 8;
			if(tid > 24)
				tid = 8;
		}
	}
	
	else{
		tid = 0;
	}
	
	
	if(key_hit(KEY_A)){
		isJumping = true;
	}
	if(isJumping == true)
		jump();
	scroll_screen();
	
	//position sprite
	obj_set_pos(mario, mx, my);
}

void update_screen(){
	mario->attr2= ATTR2_BUILD(tid, pb, 0);
	oam_copy(oam_mem, obj_buffer, 1);	//update OAM
}

int main()
{
	int selectedsong = 1;
	
	// Init interrupts and VBlank irq.
	irq_init(NULL);
	irq_add(II_VBLANK, mmVBlank);
	mmInitDefault( (mm_addr)soundbank_bin, 8 );
	
	mmStart( selectedsong, MM_PLAY_LOOP );	
	init();
	
	
	while(1)
	{
		vid_vsync();
		move_mario();
		
		update_screen();
		
		mmFrame();
		frame++;
	}
	
	return 0;
}