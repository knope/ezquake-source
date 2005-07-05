//
// HUD commands
//

#ifndef __hud_h__
#define __hud_h__

// flags
#define  HUD_NO_DRAW            (1 <<  0)  // don't draw this automatically
#define  HUD_NO_SHOW            (1 <<  1)  // doesn't support show/hide
#define  HUD_NO_POS_X           (1 <<  2)  // doesn't support x positioning
#define  HUD_NO_POS_Y           (1 <<  3)  // doesn't support y positioning
#define  HUD_NO_FRAME           (1 <<  4)  // don't add frame
#define  HUD_ON_DIALOG          (1 <<  5)  // draw on dialog too
#define  HUD_ON_INTERMISSION    (1 <<  6)  // draw on intermission too
#define  HUD_ON_FINALE          (1 <<  7)  // draw on finale too
#define  HUD_ON_SCORES          (1 <<  8)  // draw on +showscores too
#define  HUD_NO_GROW            (1 <<  9)  // no frame grow
#define  HUD_PLUSMINUS          (1 << 10)  // auto add +/- commands

#define  HUD_INVENTORY          (HUD_NO_GROW)   // aply for sbar elements

#define  HUD_MAX_PARAMS  16

typedef struct hud_s
{
    char *name;               // element name
    char *description;        // little help

    void (*draw_func) (struct hud_s *); // drawing func

    int draw_order;           // higher it is, later this element will be drawn
                              // and more probable that will be on top

    cvar_t *show;             // show cvar
    cvar_t *frame;            // frame cvar

    // placement
    cvar_t *place;            // place string, parsed to:
    struct hud_s *place_hud;  // if snapped to hud element
    qboolean place_outside;   // if hud: inside ot outside
    int place_num;            // place number (our or parent if hud)
                              // note: item is placed at another HUD element
                              // if place_hud != NULL

    cvar_t *align_x;          // alignment cvars (left, right, ...)
    cvar_t *align_y;
    int    align_x_num;       // parsed alignment
    int    align_y_num;

    cvar_t *pos_x;            // position cvars
    cvar_t *pos_y;

    cvar_t *params[HUD_MAX_PARAMS];
    int num_params;

    cactive_t  min_state;     // at least this state is required
                              // to draw this element

    unsigned   flags;

    // last draw parameters (mostly used by children)
    int lx, ly, lw, lh;       // last position
    int al, ar, at, ab;       // last frame params

    int last_try_sequence;    // sequence, at which object tried to draw itself
    int last_draw_sequence;   // sequence, at which it was last drawn successfully

    struct hud_s *next;
} hud_t;

typedef  void (*hud_func_type) (struct hud_s *);

#define MAX_HUD_ELEMENTS 256

// initialize
void HUD_Init(void);

// add element to list
// parameter format: "name1", "default1", ..., "nameX", "defaultX", NULL
hud_t * HUD_Register(char *name, char *var_alias, char *description,
                     int flags, cactive_t min_state, int draw_order,
                     hud_func_type draw_func,
                     char *show, char *place, char *align_x, char *align_y,
                     char *pos_x, char *pos_y, char *frame,
                     char *params, ...);

// draw all active elements
void HUD_Draw(void);

// retrieve hud cvar
cvar_t *HUD_FindVar(hud_t *hud, char *subvar);

// find element in list
hud_t * HUD_Find(char *name);

// calculate screen position of element
// return value:
//    true  - draw it
//    false - don't draw, it is off screen (mayby partially)
qboolean HUD_PrepareDraw(
    /* in  */ hud_t *hud, int width, int height,
    /* out */ int *ret_x, int *ret_y);

qboolean HUD_PrepareDrawByName(
    /* in  */ char *element, int width, int height,
    /* out */ int *ret_x, int *ret_y);


// last phase of initialization
void HUD_InitFinish(void);

#endif // __hud_h__
