#pragma once

#include "PluginProcessor.h"
#include <juce_opengl/juce_opengl.h>

class shaderC : public juce::OpenGLAppComponent
{
public:
    shaderC(AudioPluginAudioProcessor& p);
    ~shaderC() override;

    //==============================================================================
    // void paint (juce::Graphics&) override;
    void resized() override;
    void initialise() override; 
    void shutdown() override; 
    void render() override;

private:

    AudioPluginAudioProcessor& processorRef;

    std::unique_ptr<juce::OpenGLShaderProgram> shader;
    GLuint VBO, VAO;
    GLuint dataTextureID = 0;
    int textureWidth;
    float startTime = juce::Time::getMillisecondCounterHiRes();

    juce::String vertexSource = 
        R"glsl(#version 120
    attribute vec3 aPos;
    void main() {
        gl_Position = vec4(aPos, 1.0);
    }
    )glsl" ;

    juce::String fragmentSource =
         R"glsl(#version 120
        uniform sampler2D uDataTexture;
        uniform vec2 uRes;
        uniform float steps;

        void main() {

            vec2 uv = gl_FragCoord.xy / uRes;

            float height = texture2D(uDataTexture, vec2(uv.x*0.5, 0.5)).r*2/3;

            float no = uv.x*steps*0.5 - floor(uv.x*steps*0.5);

            float isVisible = floor(height/uv.y);

            gl_FragColor = vec4(no, isVisible, 0.0, 1.0);
        }
    )glsl" ;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (shaderC)
};

//==============================================================================
class AudioPluginAudioProcessorEditor final : public juce::AudioProcessorEditor, public juce::Timer
{
public:
    explicit AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor&);
    ~AudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;

private:

    AudioPluginAudioProcessor& processorRef;

    shaderC sh;

    size_t size = 512;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};

