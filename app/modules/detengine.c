// app/modules/detengine.c
#include "lua.h"
#include "lauxlib.h"
#include <math.h>

// —— Your C functions ——

// Decision‐tree classifier: returns 1 or 0
static int classifyBehavior(double execTime, unsigned int freeHeap) {
  if (execTime <= 25025.50) {
    if (freeHeap <= 48073.50) {
      if (freeHeap <= 46588.00) {
        return 1;
      } else {
        return 0;
      }
    } else {
      if (freeHeap <= 49260.00) {
        return 1;
      } else {
        return 0;
      }
    }
  } else {
    if (freeHeap <= 51641.50) {
      return 1;
    } else {
      return 0;
    }
  }
}

// Bayesian‐update formula
static double computeUpdateScore(double P, double d) {
  double e_md  = exp(-d);
  double numer = e_md * P;
  double denom = 1.0 - e_md - P + 2.0 * P * e_md;
  return (denom != 0.0) ? (numer / denom) : 0.0;
}

// Dempster–Shafer fusion
static double combineDST(double b, double u) {
  double K     = b * (1.0 - u) + (1.0 - b) * u;
  double denom = 1.0 - K;
  return (fabs(denom) > 1e-9) ? ((b * u) / denom) : 0.0;
}


// —— Lua bindings ——

// detengine.classify(execTime, freeHeap)
static int l_classify(lua_State *L) {
  double execTime   = luaL_checknumber(L, 1);
  unsigned int heap = (unsigned int)luaL_checkinteger(L, 2);
  int cls = classifyBehavior(execTime, heap);
  lua_pushinteger(L, cls);
  return 1;
}

// detengine.updateScore(P_theta, d_value)
static int l_updateScore(lua_State *L) {
  double P = luaL_checknumber(L, 1);
  double d = luaL_checknumber(L, 2);
  double score = computeUpdateScore(P, d);
  lua_pushnumber(L, score);
  return 1;
}

// detengine.fuseScore(bh_score, upd_score)
static int l_fuseScore(lua_State *L) {
  double b = luaL_checknumber(L, 1);
  double u = luaL_checknumber(L, 2);
  double score = combineDST(b, u);
  lua_pushnumber(L, score);
  return 1;
}


// —— Module registration ——

// Lua: require("detengine")
LUALIB_API int luaopen_detengine(lua_State *L) {
  static const luaL_Reg detengine_funcs[] = {
    {"classify",   l_classify},
    {"updateScore",l_updateScore},
    {"fuseScore",  l_fuseScore},
    {NULL, NULL}
  };
  luaL_newlib(L, detengine_funcs);
  return 1;
}
