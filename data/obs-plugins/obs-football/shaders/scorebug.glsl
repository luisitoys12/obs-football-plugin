// Scorebug GLSL Shader — OBS Football Plugin
// Renders the broadcast-style score bug with gradient background

uniform float4x4 ViewProj;
uniform texture2d image;

// Theme colors passed as uniforms
uniform float4 bugBgColor;     // Background color of the bug
uniform float4 bugAccentColor; // Accent strip
uniform float4 scoreColor;     // Score number color
uniform float time;            // For animations
uniform bool goalFlash;        // Trigger flash animation

struct VertIn {
    float4 pos : POSITION;
    float2 uv  : TEXCOORD0;
};

struct VertOut {
    float4 pos : POSITION;
    float2 uv  : TEXCOORD0;
};

VertOut VSDefault(VertIn v_in)
{
    VertOut vert_out;
    vert_out.pos = mul(float4(v_in.pos.xyz, 1.0), ViewProj);
    vert_out.uv  = v_in.uv;
    return vert_out;
}

float4 PSScoreBug(VertOut v_in) : TARGET
{
    float2 uv = v_in.uv;

    // Gradient background: dark at top, slightly lighter at bottom
    float4 baseColor = lerp(bugBgColor, bugAccentColor, uv.y * 0.5);

    // Goal flash: pulse gold overlay
    if (goalFlash) {
        float pulse = abs(sin(time * 6.0)) * 0.6;
        float4 gold = float4(1.0, 0.85, 0.0, pulse);
        baseColor   = lerp(baseColor, gold, pulse);
    }

    // Subtle vignette on edges
    float edgeFade = smoothstep(0.0, 0.08, uv.x) *
                     smoothstep(1.0, 0.92, uv.x) *
                     smoothstep(0.0, 0.08, uv.y) *
                     smoothstep(1.0, 0.92, uv.y);
    baseColor.a *= edgeFade;

    // Top accent line (broadcast style)
    if (uv.y < 0.04) {
        return float4(scoreColor.rgb, 1.0);
    }

    return baseColor;
}

technique Draw {
    pass {
        vertex_shader = VSDefault(v_in);
        pixel_shader  = PSScoreBug(v_in);
    }
}
