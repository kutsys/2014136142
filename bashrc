# ~/.bashrc : 비로그인 쉘들을 위해 bash(1)에 의해  실행된다.
# bash-doc 패키지에 있는 usr/share/doc/bash/examples/startup-files를 보아라.
# 예를 들어서


# 만약 쌍방향으로 작동하지 않는다면, 아무것도 하지 말아라.
case $- in
    *i*) ;;
      *) return;;
esac

# 중복된 라인이나 빈 공간으로 시작하는 라인을 히스토리에 넣지 말아라.
# 더 많은 옵션에 대해서는 bash(1)을 보아라.
HISTCONTROL=ignoreboth

# 히스토리 파일에 첨부하라, 덮어쓰진 않는다.
shopt -s histappend

# 히스토리 길이를 설정하고 싶다면 bash(1)의 HISTSIZE와 HISTFILESIZE를 보아라.
HISTSIZE=1000
HISTFILESIZE=2000

# 각각의 명령 이후 윈도우 사이즈를 체크하시오. 만약 필요하다면,
# 행과 열의 값을 갱신하라.
shopt -s checkwinsize

# 설정했다면, 패스이름 확장 문맥에서 사용되는 패턴 "**"는 
# 모든 파일과 0 또는 다른 디렉토리나 서브디렉토리와 매치가 될 것이다.
#shopt -s globstar

# 텍스트 입력이 없는 파일들을 위하여 less를 더 친근하게 만들고, lesspipe(1)을 보라.
[ -x /usr/bin/lesspipe ] && eval "$(SHELL=/bin/sh lesspipe)"

# 당신이 작업하는 chroot를 식별하는 변수를 설정하라. (프롬프트 밑에서 사용되는)
if [ -z "${debian_chroot:-}" ] && [ -r /etc/debian_chroot ]; then
    debian_chroot=$(cat /etc/debian_chroot)
fi

# 화려한 프롬프트를 설정하라 (우리가 색을 원한다는 것을 알지 못한다면 색은 없습니다.)
case "$TERM" in
    xterm-color) color_prompt=yes;;
esac

# 터미널이 능력이 된다면 색이 있는 프롬프트에 주석을 해제한다.
# 유저에게 혼란을 주지 않기 위하여 디폴트 값을 해제한다. 터미널 창에서 초점은
# 프롬프트가 아니라 명령의 산출물에 있어야 한다.
#force_color_prompt=yes

if [ -n "$force_color_prompt" ]; then
    if [ -x /usr/bin/tput ] && tput setaf 1 >&/dev/null; then
	# 우리는 색상 지원을 가지고 있다; 이것은 Ecma-48과 부합하다고 가정한다.
	# (ISO/IEC-6429). ( 몇몇의 지원의 부족은 극히 드물고, 그러한
	# 경우는 setaf보다 setf를 지원할 경향이 있다.)
	color_prompt=yes
    else
	color_prompt=
    fi
fi

if [ "$color_prompt" = yes ]; then
    PS1='${debian_chroot:+($debian_chroot)}\[\033[01;32m\]\u@\h\[\033[00m\]:\[\033[01;34m\]\w\[\033[00m\]\$ '
else
    PS1='${debian_chroot:+($debian_chroot)}\u@\h:\w\$ '
fi
unset color_prompt force_color_prompt

# 만약 이것이 xterm인 경우, 타이틀을 user@host:dir로 설정하라.
case "$TERM" in
xterm*|rxvt*)
    PS1="\[\e]0;${debian_chroot:+($debian_chroot)}\u@\h: \w\a\]$PS1"
    ;;
*)
    ;;
esac

# ls에 색상 지원이 가능하게 하고 간단한 명령을 추가하라.
if [ -x /usr/bin/dircolors ]; then
    test -r ~/.dircolors && eval "$(dircolors -b ~/.dircolors)" || eval "$(dirco
lors -b)"
    alias ls='ls --color=auto'
    #alias dir='dir --color=auto'
    #alias vdir='vdir --color=auto'

    alias grep='grep --color=auto'
    alias fgrep='fgrep --color=auto'
    alias egrep='egrep --color=auto'
fi

# 더 많은 ls 명령
alias ll='ls -alF'
alias la='ls -A'
alias l='ls -CF'

# 오래 작동하는 명령을 위하여 "alter"명령을 추가한다. 다음과 같이 사용한다.
#   sleep 10; alert
alias alert='notify-send --urgency=low -i "$([ $? = 0 ] && echo terminal || echo
 error)" "$(history|tail -n1|sed -e '\''s/^\s*[0-9]\+\s*//;s/[;&|]\s*alert$//'\'
')"'

# 명령 정의.
# 당신은 여기에 직접 넣는 것 대신 ~/.bash_aliases처럼 분리된 파일에 당신의 추가물들을 넣길 원할지도 모른다. 
# bash-doc 패키지 안의 /usr/share/doc/bash-doc/examples를 보아라.
if [ -f ~/.bash_aliases ]; then
    . ~/.bash_aliases
fi

# 사용 가능하고 프로그래밍 가능한 마지막 특징 
# (만약 이것이 이미 /etc/bash.bashrc and /etc/profile
# sources /etc/bash.bashrc에서 가능하다면 이것을 사용 가능하게 할 필요는 없다.)
if ! shopt -oq posix; then
  if [ -f /usr/share/bash-completion/bash_completion ]; then
    . /usr/share/bash-completion/bash_completion
  elif [ -f /etc/bash_completion ]; then
    . /etc/bash_completion
  fi
fi
