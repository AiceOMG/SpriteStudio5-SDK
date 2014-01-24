//
#include "bind_SsAnimeDecoder.h"


#include "ssplayer_animedecode.h"



Bind_SsAnimeDecoder::Bind_SsAnimeDecoder() : m_decoder(0)
{
	puts("construct Bind_SsAnimeState");	
	//m_decoder = new SsAnimeDecoder();

}

Bind_SsAnimeDecoder::~Bind_SsAnimeDecoder()
{
	puts( "destruct Bind_SsAnimeState" );
}



void Bind_SsAnimeDecoder::setFrame( int f )
{
	m_decoder->setPlayFrame( f );
}
void Bind_SsAnimeDecoder::update()
{
	m_decoder->update();
}


bool Bind_SsAnimeDecoder::debug()
{
	puts( "debug Bind_SsAnimeState" );

	if ( m_decoder )
	{
		PYDEBUG_PRINTF( "decoder:anime endframe = %d" , this->m_decoder->getAnimeEndFrame() );

		//データを出してみる
		std::list<SsPartState*>& llist = this->m_decoder->getPartSortList();

		PYDEBUG_PRINTF( "==position output test==");
		int p_no = 0;
		foreach( std::list<SsPartState*> , llist , e )
		{
			SsPartState* state = (*e);
			PYDEBUG_PRINTF( "[part%03d]position(%f,%f)" , p_no , state->position.x , state->position.y );
			p_no++;
		}
			
		//パーツ情報とアニメ情報が取得できる
		PYDEBUG_PRINTF( "==partAnime Check==");
		std::vector<SsPartAndAnime>& partAnimeList = this->m_decoder->getPartAnime();

		size_t n = partAnimeList.size();
		for ( size_t i = 0 ; i < partAnimeList.size() ; i++ )
		{
			SsPart*	part = partAnimeList[i].first;
			SsPartAnime* panime = partAnimeList[i].second;

			PYDEBUG_PRINTF( "[index%d][%s]" , i , part->name.c_str() );

			const SsKeyframe* key = panime->attributes[0]->findLeftKey(0);
			PYDEBUG_PRINTF( "key time = %d" , key->time );
		}

	}


	return true;
}

int	Bind_SsAnimeDecoder::getPartNum()
{
	std::vector<SsPartAndAnime>& partAnimeList = this->m_decoder->getPartAnime();

	return partAnimeList.size();
}

Bind_SsPart*	Bind_SsAnimeDecoder::getPart(int index)
{
	std::vector<SsPartAndAnime>& partAnimeList = this->m_decoder->getPartAnime();
	if ( index > partAnimeList.size() ) return 0;

	SsPart*	part = partAnimeList[index].first;
	if ( part == 0 ) return 0;


	return new Bind_SsPart(part);
}


Bind_SsPartAnime*	Bind_SsAnimeDecoder::getPartAnime(int index)
{
	std::vector<SsPartAndAnime>& partAnimeList = this->m_decoder->getPartAnime();
	if ( index > partAnimeList.size() ) return 0;

	SsPartAnime*	partAnime = partAnimeList[index].second;
	if ( partAnime == 0 ) return 0;


	return new Bind_SsPartAnime(partAnime);
}
